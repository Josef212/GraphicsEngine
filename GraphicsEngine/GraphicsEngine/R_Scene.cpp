#include "R_Scene.h"

#include <iostream>

#include "ModelLoader.h"

#include <GL/glew.h>

#include "Defs.h"
#include "Camera.h"
#include "R_Shader.h"
#include "R_Material.h"
#include "R_Model.h"

#include "Light.h"

#include "App.h"
#include "M_ResourceManager.h"

R_Scene::R_Scene(const char* name) : Resource(name, RES_SCENE)
{
	for (int i = 0; i < MAX_CAMERAS; ++i)
		m_cameras[i] = nullptr;
}


R_Scene::~R_Scene()
{
}

// TODO: Do really need this having scene loaders??
void R_Scene::Load()
{
	OnLoad();

	if (!m_defMaterial) m_defMaterial = app->resourceManager->defaultResources.simpleMat;
}

void R_Scene::Free()
{
	OnFree();

	//app->resourceManager->RemoveAllResources();
	m_models.clear();
	m_defMaterial = nullptr;

	for(auto it : m_lights)
	{
		RELEASE(it);
	}

	m_lights.clear();
}

void R_Scene::AddModel(R_Model* model)
{
	if (model) m_models.push_back(model);
}

void R_Scene::RemoveModel(R_Model* model)
{
	if(model)
	{
		auto it = std::find(m_models.begin(), m_models.end(), model);
		if(it != m_models.end()) m_models.erase(it);
	}
}

void R_Scene::RemoveModel(const char* name)
{
	if (name)
	{
		for (auto it = m_models.begin(); it != m_models.end();)
		{
			if ((*it)->GetName() == name)
			{
				it = m_models.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void R_Scene::AddCamera(Camera* cam, int index, bool setAsActive)
{
	if (index >= 0 && index < MAX_CAMERAS)
	{
		m_cameras[index] = cam;
		if (setAsActive)
		{
			m_activeCamera = cam;
			OnActiveCameraChanged();
		}
	}
}

void R_Scene::RemoveCamera(int index)
{
	if (index >= 0 && index < MAX_CAMERAS)
	{
		m_cameras[index] = nullptr;
	}
}

void R_Scene::SetActiveCamera(int index)
{
	if (index >= 0 && index < MAX_CAMERAS)
	{
		if (m_cameras[index])
		{
			m_activeCamera = m_cameras[index];
			OnActiveCameraChanged();
		}
	}
}

Camera* R_Scene::GetActiveCamera() const
{
	return m_activeCamera;
}

void R_Scene::ProcessScroll(double yoffset) const
{
	if (m_activeCamera) m_activeCamera->ProcessMouseScroll(yoffset);
}

void R_Scene::ProcessMouseMovement(double xoffset, double yoffset) const
{
	if (m_activeCamera) m_activeCamera->ProcessMouseMovement(xoffset, yoffset);
}

void R_Scene::ProcessInput(CameraMovement movement, float dt) const
{
	if (m_activeCamera) m_activeCamera->ProcessKeyboard(movement, dt);
}

void R_Scene::OnResize(int w, int h)
{
	m_viewportWidth = w; m_viewportHeight = h;

	for (int i = 0; i < MAX_CAMERAS; ++i)
		if (m_cameras[i]) m_cameras[i]->ResizeViewport(w, h);
}

R_Material* R_Scene::GetDefaultMaterial() const
{
	return m_defMaterial;
}

void R_Scene::AddLight(Light* light)
{
	if (light) m_lights.push_back(light);
}

void R_Scene::RemoveLight(Light * light)
{
	if(light)
	{
		auto it = std::find(m_lights.begin(), m_lights.end(), light);
		if(it != m_lights.end())
		{
			RELEASE((*it));
			m_lights.erase(it);
		}
	}
}

void R_Scene::RemoveLight(int index)
{
	if(index >= 0 && index < m_lights.size())
	{
		RemoveLight(m_lights[index]);
	}
}
