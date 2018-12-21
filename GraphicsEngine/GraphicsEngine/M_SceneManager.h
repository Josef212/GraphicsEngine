#ifndef __M_SCENEMANAGER_H__
#define __M_SCENEMANAGER_H__

#include "Module.h"
#include <vector>

class R_Scene;

class M_SceneManager : public Module
{
public:
	M_SceneManager();
	virtual ~M_SceneManager();

	bool Start() override;
	UpdateReturn Update(float dt) override;
	bool CleanUp() override;

	// TODO: Does really make sense to add and remove scenes here and store a list of them?? Since is a resource it's already on resource manager
	void AddScene(R_Scene* sc, bool activate = false);
	void RemoveScene(R_Scene* sc);
	void RemoveScene(const char* name);

	void CreateScene(const char* name);

	void SelectActiveScene(int index);
	void SelectActiveScene(const char* name);
	void SelectActiveScene(R_Scene* sc);
	R_Scene* GetActiveScene()const { return activeScene; }

	void OnResize(uint w, uint h) override;

private:
	std::vector<R_Scene*> scenes;
	R_Scene* activeScene = nullptr;
};

#endif // !__M_SCENEMANAGER_H__