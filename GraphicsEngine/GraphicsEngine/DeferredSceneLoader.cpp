#include "DeferredSceneLoader.h"

#include "App.h"
#include "M_ResourceManager.h"

#include "R_Scene.h"
#include "R_Geometry.h"
#include "R_Shader.h"
#include "R_Model.h"
#include "R_Texture.h"
#include "R_Material.h"
#include "R_Mat_SimpleTexture.h"

#include "Camera.h"
#include "PerspectiveCamera.h"

#include "Light.h"


DeferredSceneLoader::DeferredSceneLoader()
{
}


DeferredSceneLoader::~DeferredSceneLoader()
{
}

void DeferredSceneLoader::LoadScene(R_Scene* scene)
{
	if (!scene) return;

	PerspectiveCamera* camera = new PerspectiveCamera(glm::vec3(0.f, 0.f, 5.f));
	//auto model = LoadModel("./Data/Models/Knife/Knife.obj", scene);
	auto model = LoadModel("./Data/Models/iPhone5/iPhone5.obj", scene);
	//model->SetScale(glm::vec3(0.05f));

	scene->AddCamera(camera, 0, true);
}
