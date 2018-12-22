#include "SceneLoader.h"

#include "R_Scene.h"
#include "ModelLoader.h"

void SceneLoader::LoadModel(const char * path, R_Scene * scene)
{
	if (path)
	{
		ModelLoader::LoadModel(path, scene);
	}
}
