#include "R_Model.h"

#include "M_ResourceManager.h"
#include "R_Shader.h"
#include "R_Geometry.h"
#include "R_Material.h"
#include "R_Scene.h"
#include "Camera.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "App.h"


R_Model::R_Model(const char* name) : Resource(name, RES_MODEL)
{
	m_modelMat = glm::mat4(1.f);
}

R_Model::R_Model(const char* name, R_Geometry* geo, R_Material* mat) : Resource(name, RES_MODEL)
{
	m_modelMat = glm::mat4(1.f);

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
	m_meshes.emplace_back(geo, mat);
}

void R_Model::SetGeometry(R_Geometry * geo, int index)
{
	if (index >= 0 && index < m_meshes.size())
	{
		m_meshes[index].first = geo;
	}
}

void R_Model::SetMaterial(R_Material * mat, int index)
{
	if (index >= 0 && index < m_meshes.size())
	{
		m_meshes[index].second = mat;
	}
}

void R_Model::SetMaterial(R_Material * mat)
{
	if (mat)
	{
		for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
		{
			(*it).second = mat;
		}
	}
}

std::pair<R_Geometry*, R_Material*> R_Model::GetMesh(int index) const
{
	if (index >= 0 && index < m_meshes.size())
	{
		return m_meshes[index];
	}

	return std::pair<R_Geometry*, R_Material*>(nullptr, nullptr);
}

R_Geometry * R_Model::GetGeometry(int index) const
{
	if (index >= 0 && index < m_meshes.size())
	{
		return m_meshes[index].first;
	}

	return nullptr;
}

R_Material * R_Model::GetMaterial(int index) const
{
	if (index >= 0 && index < m_meshes.size())
	{
		return m_meshes[index].second;
	}

	return nullptr;
}

glm::vec3 R_Model::GetTranslation() const
{
	return m_translation;
}

void R_Model::SetTranslation(glm::vec3 pos)
{
	m_translation = pos;
	CalcModel();
}

glm::vec3 R_Model::GetEuler() const
{
	return m_eulerRot;
}

void R_Model::SetEuler(glm::vec3 euler)
{
	m_eulerRot = euler;
	CalcModel();
}

glm::vec3 R_Model::GetScale() const
{
	return m_scale;
}

void R_Model::SetScale(glm::vec3 scl)
{
	m_scale = scl;
	CalcModel();
}

void R_Model::CalcModel()
{
	m_modelMat = glm::mat4(1.f);

	m_modelMat = glm::scale(m_modelMat, m_scale);

	m_modelMat = glm::rotate(m_modelMat, glm::radians(m_eulerRot.x), glm::vec3(1.f, 0.f, 0.f));
	m_modelMat = glm::rotate(m_modelMat, glm::radians(m_eulerRot.y), glm::vec3(0.f, 1.f, 0.f));
	m_modelMat = glm::rotate(m_modelMat, glm::radians(m_eulerRot.z), glm::vec3(0.f, 0.f, 1.f));

	m_modelMat = glm::translate(m_modelMat, m_translation);
}
