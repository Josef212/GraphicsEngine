#include "BasicResourcesSceneLoader.h"

#include "App.h"
#include "M_ResourceManager.h"

#include "R_Scene.h"
#include "R_Geometry.h"
#include "R_Shader.h"
#include "R_Material.h"
#include "R_Model.h"
#include "R_Texture.h"

#include "Camera.h"
#include "PerspectiveCamera.h"

BasicResourcesSceneLoader::BasicResourcesSceneLoader()
{
}


BasicResourcesSceneLoader::~BasicResourcesSceneLoader()
{
}

void BasicResourcesSceneLoader::LoadScene(R_Scene * scene)
{
	if (!scene) return;

	R_Shader* basicShader = new R_Shader("Basic shader", "./Data/Shaders/basic.vert", "./Data/Shaders/basic.frag");
	R_Material* basicMaterial = new R_Material("Basic material", basicShader);
	R_Geometry* quad = app->resourceManager->defaultResources.quadGeo;
	R_Model* model = new R_Model("Basic quad", quad, basicMaterial);

	PerspectiveCamera* camera = new PerspectiveCamera(glm::vec3(0.f, 0.f, 5.f));

	scene->AddModel(model);
	scene->AddCamera(camera, 0, true);
}
