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

	R_Scene* CreateScene(const char* name, bool activate = false);

	void SelectActiveScene(int index);
	void SelectActiveScene(const char* name);
	void SelectActiveScene(R_Scene* sc);
	R_Scene* GetActiveScene()const { return m_activeScene; }

	void OnResize(uint w, uint h) override;

private:
	std::vector<R_Scene*> m_scenes;
	R_Scene* m_activeScene = nullptr;

	int lastX = INT_MAX, lastY = INT_MAX;
};

#endif // !__M_SCENEMANAGER_H__