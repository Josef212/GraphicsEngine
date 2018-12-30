#include "BasicResourcesSceneLoader.h"

#include "App.h"
#include "M_ResourceManager.h"

#include "R_Scene.h"
#include "R_Geometry.h"
#include "R_Shader.h"
#include "R_Model.h"
#include "R_Texture.h"
#include "R_Material.h"
#include "R_Mat_SimpleTexture.h"
#include "R_ComplexMaterial.h"

#include "Camera.h"
#include "PerspectiveCamera.h"

#include "Light.h"

BasicResourcesSceneLoader::BasicResourcesSceneLoader()
{
}


BasicResourcesSceneLoader::~BasicResourcesSceneLoader()
{
}

void BasicResourcesSceneLoader::LoadScene(R_Scene * scene)
{
	if (!scene) return;

	R_Texture* iPhoneDiffuse = new R_Texture("iPhone diffuse");
	R_Texture* iPhoneSpecular = new R_Texture("iPhone specular");
	iPhoneDiffuse->LoadTexture("./Data/Models/iPhone5/Iphone5_diffuse.png");
	iPhoneSpecular->LoadTexture("./Data/Models/iPhone5/Iphone5_specular.png");

	R_Shader* iPhoneSh = new R_Shader("iPhone shader", "./Data/Shaders/difspec.vert", "./Data/Shaders/difspec.frag");
	R_ComplexMaterial* iPhoneMat = new R_ComplexMaterial("iPhone material", iPhoneSh);
	iPhoneMat->AddProperty(new MatProperty("diffuse", iPhoneDiffuse));
	iPhoneMat->AddProperty(new MatProperty("specular", iPhoneSpecular));

	auto iPhone = LoadModel("./Data/Models/iPhone5/iPhone5.obj", scene);
	iPhone->SetScale(glm::vec3(0.01f));
	iPhone->SetMaterial(iPhoneMat);

	R_Geometry* quad = app->resourceManager->defaultResources.quadGeo;
	R_Shader* textureShader = new R_Shader("Basic shader", "./Data/Shaders/basic.vert", "./Data/Shaders/texture.frag");
	R_Mat_SimpleTexture* material = new R_Mat_SimpleTexture("Simple texture mat");
	material->SetShader(textureShader);
	material->texture = app->resourceManager->defaultResources.lennaTex;
	R_Model* model = new R_Model("Basic quad", quad, material);
	
	PerspectiveCamera* camera = new PerspectiveCamera(glm::vec3(0.f, 0.f, 5.f));
	scene->AddCamera(camera, 0, true);

	auto m = LoadModel("./Data/Models/boat.fbx", scene);
	m->SetMaterial(material);
	m->SetScale(glm::vec3(0.05f));

	// Lights
	Light* l0 = new Light();
	Light* l1 = new Light();
	Light* l2 = new Light();

	l0->m_position = glm::vec3(2.f, 1.f, -3.f);
	l1->m_position = glm::vec3(-2.f, -1.f, -1.f);
	l2->m_position = glm::vec3(-5.f, 2.f, -5.f);

	l0->m_color = glm::vec3(1.f, 0.f, 0.f);
	l1->m_color = glm::vec3(0.f, 1.f, 0.f);
	l2->m_color = glm::vec3(0.f, 0.f, 1.f);

	l0->m_type = LightType::L_POINT;
	l1->m_type = LightType::L_POINT;
	l2->m_type = LightType::L_POINT;

	scene->AddLight(l0);
	scene->AddLight(l1);
	scene->AddLight(l2);
}
