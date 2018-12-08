#include "M_SceneManager.h"

#include "Defs.h"
#include "R_Scene.h"
#include "App.h"
#include "M_ResourceManager.h"


M_SceneManager::M_SceneManager() : Module("M_SceneManager", true)
{
}


M_SceneManager::~M_SceneManager()
{
}

bool M_SceneManager::Start()
{
	return true;
}

UpdateReturn M_SceneManager::Update(float dt)
{
	return UPDT_CONTINUE;
}

bool M_SceneManager::CleanUp()
{
	return true;
}

void M_SceneManager::AddScene(R_Scene * sc, bool activate)
{
	if (!sc) return;

	scenes.push_back(sc);

	if(activate)
	{
		SelectActiveScene(sc);
	}
}

void M_SceneManager::RemoveScene(R_Scene * sc)
{
	if (sc)
	{
		for (auto it = scenes.begin(); it != scenes.end();)
		{
			if ((*it) == sc)
			{
				app->resourceManager->RemoveResource((*it));
				it = scenes.erase(it);
			}
		}
	}
}

void M_SceneManager::RemoveScene(const char * name)
{
	if (name)
	{
		for (auto it = scenes.begin(); it != scenes.end();)
		{
			if ((*it)->GetName() == name)
			{
				app->resourceManager->RemoveResource((*it));
				it = scenes.erase(it);
			}
		}
	}
}

void M_SceneManager::CreateScene(const char * name)
{
	R_Scene* sc = new R_Scene(name);
	AddScene(sc);
}

void M_SceneManager::SelectActiveScene(int index)
{
	if (index >= 0 && index < scenes.size())
	{
		if (activeScene) activeScene->Free();
		activeScene = scenes[index];
		activeScene->Load();
	}
}

void M_SceneManager::SelectActiveScene(const char * name)
{
	if (name)
	{
		for (auto it : scenes)
		{
			if (it->GetName() == name)
			{
				if (activeScene) activeScene->Free();
				activeScene = it;
				activeScene->Load();
			}
		}
	}
}

void M_SceneManager::SelectActiveScene(R_Scene * sc)
{
	auto it = std::find(scenes.begin(), scenes.end(), sc);
	if (it != scenes.end())
	{
		if (activeScene) activeScene->Free();
		activeScene = sc;
		activeScene->Load();
	}
	else
	{
		if(sc)
		{
			AddScene(sc, true);
		}
		else
		{
			if (activeScene) activeScene->Free();
			activeScene = nullptr;
		}
	}
}

void M_SceneManager::OnResize(uint w, uint h)
{
	for (auto it : scenes)
	{
		it->OnResize(w, h);
	}
}
