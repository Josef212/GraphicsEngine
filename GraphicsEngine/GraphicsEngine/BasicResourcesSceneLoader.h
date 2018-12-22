#ifndef __BASICRESOURCESSCENELOADER_H__
#define __BASICRESOURCESSCENELOADER_H__

#include "SceneLoader.h"

class BasicResourcesSceneLoader : public SceneLoader
{
public:
	BasicResourcesSceneLoader();
	virtual ~BasicResourcesSceneLoader();

	static void LoadScene(R_Scene* scene);
};

#endif // !__BASICRESOURCESSCENELOADER_H__