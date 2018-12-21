#ifndef __SCENELOADER_H__
#define __SCENELOADER_H__

class R_Scene;

static class SceneLoader
{
public:
	SceneLoader();
	virtual ~SceneLoader();

	static void LoadScene(R_Scene* scene) {}
};

#endif // !__SCENELOADER_H__