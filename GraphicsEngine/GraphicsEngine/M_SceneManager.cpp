#include "M_SceneManager.h"

#include "EventManager.h"

#include "Defs.h"
#include "R_Scene.h"
#include "App.h"
#include "M_ResourceManager.h"
#include "M_Input.h"
#include "M_Editor.h"
#include "Camera.h"

#include <SDL_scancode.h>
#include <SDL_mouse.h>

// TMP
#include "BasicResourcesSceneLoader.h"


M_SceneManager::M_SceneManager() : Module("M_SceneManager", true)
{
	LOG_CREATION(m_moduleName.c_str());

	m_configuration = M_START | M_UPDATE | M_CLEAN_UP; 
	// TODO: Probably update not needed although it mught update the active scene
}


M_SceneManager::~M_SceneManager()
{
	LOG_DESTRUCTION(m_moduleName.c_str());
}

bool M_SceneManager::Start()
{
	LOG_START(m_moduleName.c_str());

	app->eventManager->AddEventListener(this);

	// TMP
	auto sc = CreateScene("Simple scene", true);
	BasicResourcesSceneLoader::LoadScene(sc);

	return true;
}

UpdateReturn M_SceneManager::Update(float dt)
{
	if (!m_activeScene) return UpdateReturn::UPDT_CONTINUE;

	if (!app->editor->UsingKeyboard())
	{
		M_Input* input = app->input;
	
		float _dt = input->GetKey(SDL_SCANCODE_LSHIFT) ? dt * 2.f : dt;
	
		if (input->GetKey(SDL_SCANCODE_W)) //w
			m_activeScene->ProcessInput(CameraMovement::FORWARD, _dt);
		if (input->GetKey(SDL_SCANCODE_S)) //s
			m_activeScene->ProcessInput(CameraMovement::BACKWARD, _dt);
		if (input->GetKey(SDL_SCANCODE_A)) //a
			m_activeScene->ProcessInput(CameraMovement::LEFT, _dt);
		if (input->GetKey(SDL_SCANCODE_D)) //d
			m_activeScene->ProcessInput(CameraMovement::RIGHT, _dt);
		if (input->GetKey(SDL_SCANCODE_Q)) //q
			m_activeScene->ProcessInput(CameraMovement::UP, _dt);
		if (input->GetKey(SDL_SCANCODE_E)) //e
			m_activeScene->ProcessInput(CameraMovement::DOWN, _dt);
	}
	
	if (!app->editor->UsingMouse())
	{
		if (app->input->GetMouseButton(SDL_BUTTON_LEFT))
		{
			int x = app->input->GetMouseX();
			int y = app->input->GetMouseY();

			if (lastX == INT_MAX || lastY == INT_MAX)
			{
				lastX = x;
				lastY = y;
			}

			float dx = x - lastX;
			float dy = lastY - y;

			lastX = x;
			lastY = y;
			
			m_activeScene->ProcessMouseMovement(dx, dy);
		}
		
		m_activeScene->ProcessScroll(app->input->GetMouseWheel());
	}

	if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		lastX = INT_MAX;
	}

	return UPDT_CONTINUE;
}

bool M_SceneManager::CleanUp()
{
	LOG_CLEANUP(m_moduleName.c_str());

	app->eventManager->RemoveEventListener(this);

	return true;
}

void M_SceneManager::AddScene(R_Scene * sc, bool activate)
{
	if (!sc) return;

	m_scenes.push_back(sc);

	if(activate)
	{
		SelectActiveScene(sc);
	}
}

void M_SceneManager::RemoveScene(R_Scene * sc)
{
	if (sc)
	{
		for (auto it = m_scenes.begin(); it != m_scenes.end();)
		{
			if ((*it) == sc)
			{
				app->resourceManager->RemoveResource((*it));
				it = m_scenes.erase(it);
			}
		}
	}
}

void M_SceneManager::RemoveScene(const char * name)
{
	if (name)
	{
		for (auto it = m_scenes.begin(); it != m_scenes.end();)
		{
			if ((*it)->GetName() == name)
			{
				app->resourceManager->RemoveResource((*it));
				it = m_scenes.erase(it);
			}
		}
	}
}

R_Scene* M_SceneManager::CreateScene(const char * name, bool activate)
{
	R_Scene* sc = new R_Scene(name);
	AddScene(sc, activate);

	return sc;
}

void M_SceneManager::SelectActiveScene(int index)
{
	if (index >= 0 && index < m_scenes.size())
	{
		if (m_activeScene) m_activeScene->Free();
		m_activeScene = m_scenes[index];
		m_activeScene->Load();
	}
}

void M_SceneManager::SelectActiveScene(const char * name)
{
	if (name)
	{
		for (auto it : m_scenes)
		{
			if (it->GetName() == name)
			{
				if (m_activeScene) m_activeScene->Free();
				m_activeScene = it;
				m_activeScene->Load();
			}
		}
	}
}

void M_SceneManager::SelectActiveScene(R_Scene * sc)
{
	auto it = std::find(m_scenes.begin(), m_scenes.end(), sc);
	if (it != m_scenes.end())
	{
		if (m_activeScene) m_activeScene->Free();
		m_activeScene = sc;
		m_activeScene->Load();
	}
	else
	{
		if(sc)
		{
			AddScene(sc, true);
		}
		else
		{
			if (m_activeScene) m_activeScene->Free();
			m_activeScene = nullptr;
		}
	}
}

EventType M_SceneManager::GetSupportedEvents()
{
	return EventType::EVENT_WINDOW_RESIZE;
}

void M_SceneManager::OnEventRecieved(Event e)
{
	if (e.type == EventType::EVENT_WINDOW_RESIZE)
	{
		for (auto it : m_scenes)
		{
			it->OnResize(e.data._v2.x, e.data._v2.y);
		}
	}
}