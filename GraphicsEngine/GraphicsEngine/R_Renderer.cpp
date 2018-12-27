#include "R_Renderer.h"

#include "App.h"
#include "M_ResourceManager.h"

#include "R_Scene.h"
#include "R_Model.h"
#include "R_Geometry.h"
#include "R_Material.h"
#include "R_Shader.h"

#include "Light.h"
#include "Camera.h"

#include "OpenGL.h"
#include <glm/gtc/matrix_transform.hpp>

#define LIGHTS_DEBUG_SCALE 0.2f

R_Renderer::R_Renderer(const char* name) : Resource(name, RES_RENDERER)
{
}


R_Renderer::~R_Renderer()
{
}

void R_Renderer::ClearPass(R_Scene* scene)
{
	glClearColor(scene->m_backgroundColor.r, scene->m_backgroundColor.g, scene->m_backgroundColor.b, scene->m_backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void R_Renderer::ClearGeometryBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void R_Renderer::RenderModels(R_Scene* scene, int renderConfig)
{
	for(auto model : scene->m_models)
	{
		for(auto mesh : model->m_meshes)
		{
			RenderMesh(mesh.first, mesh.second, model, scene, renderConfig);
		}
	}

	ClearGeometryBuffer();
	glUseProgram(0);
}

void R_Renderer::RenderModel(R_Model* model, R_Scene* scene, int renderConfig)
{
	for(auto mesh : model->m_meshes)
	{
		RenderMesh(mesh.first, mesh.second, model, scene, renderConfig);
	}

	ClearGeometryBuffer();
	glUseProgram(0);
}

void R_Renderer::RenderMesh(R_Geometry* geometry, R_Material* material, R_Model* model, R_Scene* scene, int renderConfig)
{
	material->InitRender();

	Camera* camera = scene->GetActiveCamera();

	material->GetShader()->SetMat4("model", renderConfig & PASS_MODEL ? model->m_modelMat : glm::mat4(1.f));
	material->GetShader()->SetMat4("view", renderConfig & PASS_MODEL ? camera->GetViewMatrix() : glm::mat4(1.f));
	material->GetShader()->SetMat4("projection", renderConfig & PASS_MODEL ? camera->GetProjectionMatrix() : glm::mat4(1.f));

	material->SendInfo(scene);

	glBindVertexArray(geometry->EBO());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->IdIndices());

	glDrawElements(GL_TRIANGLES, geometry->CountIndices(), GL_UNSIGNED_INT, NULL);
}

void R_Renderer::RenderGeometry(R_Geometry* geometry)
{
	glBindVertexArray(geometry->EBO());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->IdIndices());

	glDrawElements(GL_TRIANGLES, geometry->CountIndices(), GL_UNSIGNED_INT, NULL);
}

void R_Renderer::DebugRenderLights(R_Scene* scene)
{
	R_Geometry* cube = app->resourceManager->defaultResources.cubeGeo;
	R_Material* material = app->resourceManager->defaultResources.debugLightsMat;
	Camera* camera = scene->GetActiveCamera();

	material->InitRender();
	R_Shader* shader = material->GetShader();

	shader->SetMat4("projection", camera->GetProjectionMatrix());
	shader->SetMat4("view", camera->GetViewMatrix());

	const glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(LIGHTS_DEBUG_SCALE));

	for(auto it : *scene->GetLights())
	{
		shader->SetMat4("model", glm::translate(model, it->m_position));
		shader->SetVec3("lightColor", it->m_color);

		RenderGeometry(cube);
	}

	material->EndRender();
}
