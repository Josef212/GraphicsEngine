#include "R_Model.h"

#include "M_ResourceManager.h"
#include "R_Shader.h"
#include "R_Geometry.h"
#include "R_Material.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "App.h"


R_Model::R_Model(const char* name) : Resource(name, RES_MODEL)
{
	modelMat = glm::mat4(1.f);
}

R_Model::R_Model(const char* name, R_Geometry* geo, R_Material* mat) : Resource(name, RES_MODEL)
{
	modelMat = glm::mat4(1.f);

	AddMesh(geo, mat);
}


R_Model::~R_Model()
{
}

void R_Model::Free()
{
}

void R_Model::AddMesh(R_Geometry * geo, R_Material * mat)
{
	meshes.emplace_back(geo, mat);
}

void R_Model::SetGeometry(R_Geometry * geo, int index)
{
	if (index >= 0 && index < meshes.size())
	{
		meshes[index].first = geo;
	}
}

void R_Model::SetMaterial(R_Material * mat, int index)
{
	if (index >= 0 && index < meshes.size())
	{
		meshes[index].second = mat;
	}
}

void R_Model::SetMaterial(R_Material * mat)
{
	if (mat)
	{
		for (auto it = meshes.begin(); it != meshes.end(); ++it)
		{
			(*it).second = mat;
		}
	}
}

std::pair<R_Geometry*, R_Material*> R_Model::GetMesh(int index) const
{
	if (index >= 0 && index < meshes.size())
	{
		return meshes[index];
	}

	return std::pair<R_Geometry*, R_Material*>(nullptr, nullptr);
}

R_Geometry * R_Model::GetGeometry(int index) const
{
	if (index >= 0 && index < meshes.size())
	{
		return meshes[index].first;
	}

	return nullptr;
}

R_Material * R_Model::GetMaterial(int index) const
{
	if (index >= 0 && index < meshes.size())
	{
		return meshes[index].second;
	}

	return nullptr;
}

void R_Model::Render(int renderConfig)
{
	for (auto it : meshes)
	{
		R_Geometry* geometry = it.first;
		R_Material* material = it.second;

		if (geometry)
		{
			if (!material) material = app->resourceManager->defaultResources.simpleMat; //scene->GetDefaultMaterial();

			material->InitRender();

			material->GetShader()->SetMat4("model", renderConfig & PASS_MODEL ? modelMat : glm::mat4(1.f));
			//material->GetShader()->SetMat4("view", renderConfig & PASS_VIEW ? scene->GetActiveCamera()->GetViewMatrix() : glm::mat4(1.f));
			//material->GetShader()->SetMat4("projection", renderConfig & PASS_PROJ ? scene->GetActiveCamera()->GetProjectionMatrix() : glm::mat4(1.f));

			material->SendInfo(/*scene*/);

			glBindVertexArray(geometry->EBO());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->IdIndices());

			glDrawElements(GL_TRIANGLES, geometry->CountIndices(), GL_UNSIGNED_INT, NULL);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			material->EndRender();
		}
	}
}

glm::vec3 R_Model::GetTranslation() const
{
	return translation;
}

void R_Model::SetTranslation(glm::vec3 pos)
{
	translation = pos;
	CalcModel();
}

glm::vec3 R_Model::GetEuler() const
{
	return eulerRot;
}

void R_Model::SetEuler(glm::vec3 euler)
{
	eulerRot = euler;
	CalcModel();
}

glm::vec3 R_Model::GetScale() const
{
	return scale;
}

void R_Model::SetScale(glm::vec3 scl)
{
	scale = scl;
	CalcModel();
}

void R_Model::CalcModel()
{
	modelMat = glm::mat4(1.f);

	modelMat = glm::scale(modelMat, scale);

	modelMat = glm::rotate(modelMat, glm::radians(eulerRot.x), glm::vec3(1.f, 0.f, 0.f));
	modelMat = glm::rotate(modelMat, glm::radians(eulerRot.y), glm::vec3(0.f, 1.f, 0.f));
	modelMat = glm::rotate(modelMat, glm::radians(eulerRot.z), glm::vec3(0.f, 0.f, 1.f));

	modelMat = glm::translate(modelMat, translation);
}
