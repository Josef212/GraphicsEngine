#include "R_Scene.h"

#include <iostream>

#include "ModelLoader.h"

#include <GL/glew.h>

#include "Defs.h"
#include "Camera.h"
#include "R_Shader.h"
#include "R_Material.h"
#include "R_Model.h"

#include "App.h"
#include "M_ResourceManager.h"

R_Scene::R_Scene(const char* name) : Resource(name, RES_SCENE)
{
	for (int i = 0; i < MAX_CAMERAS; ++i)
		cameras[i] = nullptr;
}


R_Scene::~R_Scene()
{
}

void R_Scene::Load()
{
	OnLoad();

	if (!defMaterial) defMaterial = app->resourceManager->defaultResources.simpleMat;
}

void R_Scene::Free()
{
	OnFree();

	//app->resourceManager->RemoveAllResources();
	models.clear();
	defMaterial = nullptr;
}

void R_Scene::AddModel(R_Model* model)
{
	if (model) models.push_back(model);
}

void R_Scene::RemoveModel(R_Model* model)
{
	if(model)
	{
		auto it = std::find(models.begin(), models.end(), model);
		if(it != models.end()) models.erase(it);
	}
}

void R_Scene::RemoveModel(const char* name)
{
	if (name)
	{
		for (auto it = models.begin(); it != models.end();)
		{
			if ((*it)->GetName() == name)
			{
				it = models.erase(it);
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
		cameras[index] = cam;
		if (setAsActive)
		{
			activeCamera = cam;
			OnActiveCameraChanged();
		}
	}
}

void R_Scene::RemoveCamera(int index)
{
	if (index >= 0 && index < MAX_CAMERAS)
	{
		cameras[index] = nullptr;
	}
}

void R_Scene::SetActiveCamera(int index)
{
	if (index >= 0 && index < MAX_CAMERAS)
	{
		if (cameras[index])
		{
			activeCamera = cameras[index];
			OnActiveCameraChanged();
		}
	}
}

Camera* R_Scene::GetActiveCamera() const
{
	return activeCamera;
}

void R_Scene::OnResize(int w, int h)
{
	viewportWidth = w; viewportHeight = h;

	for (int i = 0; i < MAX_CAMERAS; ++i)
		if (cameras[i]) cameras[i]->ResizeViewport(w, h);
}

R_Material* R_Scene::GetDefaultMaterial() const
{
	return defMaterial;
}

// TODO: This should be done in renderer. Scene should be only a scene info container
void R_Scene::RenderScene()
{
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (GetActiveCamera()) OnRenderScene();
}

void R_Scene::ProcessScroll(double yoffset)
{
	if (activeCamera) activeCamera->ProcessMouseScroll(yoffset);
}

void R_Scene::ProcessMouseMovement(double xoffset, double yoffset)
{
	if (activeCamera) activeCamera->ProcessMouseMovement(xoffset, yoffset);
}

void R_Scene::ProcessInput(CameraMovement movement, float dt)
{
	if (activeCamera) activeCamera->ProcessKeyboard(movement, dt);
}

void R_Scene::LoadModel(const char* path)
{
	// TODO: A default implementation to load 3D models so this calss has functionality by itself
	if(path)
	{
		ModelLoader::LoadModel(path, this);
	}
}
