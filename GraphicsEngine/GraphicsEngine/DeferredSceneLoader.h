#ifndef __DEFERREDSCENELOADER_H__
#define __DEFERREDSCENELOADER_H__

#include "SceneLoader.h"

class DeferredSceneLoader : public SceneLoader
{
public:
	DeferredSceneLoader();
	virtual ~DeferredSceneLoader();

	static void LoadScene(R_Scene* scene);
};

#endif // ! __DEFERREDSCENELOADER_H__