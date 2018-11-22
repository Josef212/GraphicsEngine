#include "TestScene.h"



TestScene::TestScene() : Module("Test Scene", true, MODULE_FULL_CONFIG)
{
}


TestScene::~TestScene()
{
}

bool TestScene::Init()
{
	return true;
}

UpdateReturn TestScene::PreUpdate(float dt)
{
	return UpdateReturn::UPDT_CONTINUE;
}

UpdateReturn TestScene::Update(float dt)
{
	return UpdateReturn::UPDT_CONTINUE;
}

bool TestScene::CleanUp()
{
	return true;
}
