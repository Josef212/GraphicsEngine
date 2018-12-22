#ifndef __SCENELOADER_H__
#define __SCENELOADER_H__

class R_Scene;
class R_Model;

class SceneLoader
{
public:
	SceneLoader() {}
	virtual ~SceneLoader() = default;

	static void LoadScene(R_Scene* scene) {}

protected:
	static R_Model* LoadModel(const char* path, R_Scene* scene);
};

#endif // !__SCENELOADER_H__