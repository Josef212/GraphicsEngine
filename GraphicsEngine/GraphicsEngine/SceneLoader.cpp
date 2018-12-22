#include "SceneLoader.h"

#include "R_Scene.h"
#include "ModelLoader.h"

R_Model* SceneLoader::LoadModel(const char * path, R_Scene * scene)
{
	if (path)
	{
		return ModelLoader::LoadModel(path, scene);
	}

	return nullptr;
}
