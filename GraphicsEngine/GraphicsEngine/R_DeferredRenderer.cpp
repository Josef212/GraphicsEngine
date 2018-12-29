#include "R_DeferredRenderer.h"

#include "App.h"
#include "M_ResourceManager.h"
#include "FrameBuffer.h"
#include "M_Window.h"
#include "ThirdParty/ImGui/imgui.h"

#include "R_Scene.h"
#include "R_Geometry.h"
#include "R_Shader.h"
#include "R_Material.h"
#include "R_Model.h"
#include "R_Texture.h"

#include "Camera.h"

#include "OpenGL.h"


R_DeferredRenderer::R_DeferredRenderer(const char* name) : R_Renderer(name)
{
	m_gBuffer = new FrameBuffer(app->window->GetWinWidth(), app->window->GetWinHeight());

	TextureConfig pos, normal, albedoSpec;
	CreateDefaultColorTexture(pos);
	pos.internalFormat = GL_RGB16F; pos.type = GL_FLOAT;
	CreateDefaultColorTexture(normal);
	normal.internalFormat = GL_RGB16F; normal.type = GL_FLOAT;
	CreateDefaultColorTexture(albedoSpec);
	albedoSpec.internalFormat = GL_RGBA; albedoSpec.format = GL_RGBA;

	m_gBuffer->Bind();

	m_gBuffer->CreateTexture(pos);
	m_gBuffer->CreateTexture(normal);
	m_gBuffer->CreateTexture(albedoSpec);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	//TODO: draw buffers

	m_gBuffer->UnBind();

	m_deferredShader = new R_Shader("R_DeferredShader", "./Data/Shaders/basicDeferred.vert", "./Data/Shaders/basicDeferred.frag");
	m_frameShader = new R_Shader("R_FrameShader", "./Data/Shaders/frameShader.vert", "./Data/Shaders/frameShader.frag");

	m_diffuse = new R_Texture("R_DiffuseTex");
	m_specular = new R_Texture("R_Specular");

	m_diffuse->LoadTexture("./Data/Models/iPhone5/Iphone5_diffuse.png");
	m_specular->LoadTexture("./Data/Models/iPhone5/Iphone5_specular.png");
}


R_DeferredRenderer::~R_DeferredRenderer()
{
}

void R_DeferredRenderer::RenderScene(R_Scene* scene)
{
	assert(scene != nullptr);

	m_gBuffer->Bind();

	ClearPass(scene);

	auto camera = scene->GetActiveCamera();
	auto model = scene->GetModel(0);

	if (!camera || !model) return;

	m_deferredShader->Use();
	
	m_deferredShader->SetMat4("view", camera->GetViewMatrix());
	m_deferredShader->SetMat4("projection", camera->GetProjectionMatrix());
	m_deferredShader->SetMat4("model", model->m_modelMat);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuse->TextureID());
	m_deferredShader->SetInt("diffuse", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_specular->TextureID());
	m_deferredShader->SetInt("specular", 1);

	for(int i = 0; i < model->GetMeshCount(); ++i)
	{
		auto it = model->GetMesh(i);
		R_Geometry* geo = it.first;
		RenderGeometry(geo);
	}

	glUseProgram(0);
	m_gBuffer->UnBind();



	ImGui::Begin("Framebuffers", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	for (int i = 0; i < MAX_FB_TEXTURES; ++i)
	{
		ImGui::Image((ImTextureID)m_gBuffer->GetTexture(i), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));

		if (((i + 1) % 3) != 0) ImGui::SameLine();
	}

	ImGui::End();
}
