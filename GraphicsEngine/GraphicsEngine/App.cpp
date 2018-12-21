#include "App.h"

#include "EventManager.h"

#include "M_FileSystem.h"
#include "M_Window.h"
#include "M_Input.h"
#include "M_Editor.h"
#include "M_ResourceManager.h"
#include "M_SceneManager.h"


#include "M_Render3D.h"

#include <SDL_timer.h>

App::App()
{
	LOG(LOG_INFO, "Creating modules.");

	clock				= new GE::Clock();
	eventManager		= new EventManager();

	fs					= new M_FileSystem();
	window				= new M_Window();
	input				= new M_Input();
	editor				= new M_Editor();
	resourceManager		= new M_ResourceManager();
	sceneManager		= new M_SceneManager();

	render				= new M_Render3D();


	m_modules.push_back(fs);
	m_modules.push_back(window);
	m_modules.push_back(input);
	m_modules.push_back(editor);
	m_modules.push_back(resourceManager);
	m_modules.push_back(sceneManager);

	m_modules.push_back(render);
}


App::~App()
{
	for(auto it = m_modules.rbegin(); it != m_modules.rend(); ++it)
	{
		RELEASE((*it));
	}

	RELEASE(eventManager);
	RELEASE(clock);
}

bool App::Init()
{
	bool ret = true;

	LOG(LOG_INFO, "Initing modules.");

	auto it = m_modules.begin();
	while(it != m_modules.end() && ret)
	{
		if ((*it)->m_configuration & M_INIT) 
			ret = (*it)->Init();
		++it;
	}

	if (!ret) return ret;

	LOG(LOG_INFO, "Starting modules.");

	it = m_modules.begin();
	while (it != m_modules.end() && ret)
	{
		if ((*it)->m_configuration & M_START) 
			ret = (*it)->Start();
		++it;
	}

	return ret;
}

UpdateReturn App::Update()
{
	UpdateReturn ret = UpdateReturn::UPDT_CONTINUE;

	PrepareUpdate();

	auto it = m_modules.begin();
	while (it != m_modules.end() && ret == UpdateReturn::UPDT_CONTINUE)
	{
		if ((*it)->m_configuration & M_PRE_UPDATE && (*it)->IsEnabled()) 
			ret = (*it)->PreUpdate(clock->Dt());
		++it;
	}

	if (ret == UpdateReturn::UPDT_ERROR) LOG(LOG_ERROR, "Exiting with error on preupdate.");

	it = m_modules.begin();
	while (it != m_modules.end() && ret == UpdateReturn::UPDT_CONTINUE)
	{
		if ((*it)->m_configuration & M_UPDATE && (*it)->IsEnabled())
			ret = (*it)->Update(clock->Dt());
		++it;
	}

	if (ret == UpdateReturn::UPDT_ERROR) LOG(LOG_ERROR, "Exiting with error on update.");

	it = m_modules.begin();
	while (it != m_modules.end() && ret == UpdateReturn::UPDT_CONTINUE)
	{
		if ((*it)->m_configuration & M_POST_UPDATE && (*it)->IsEnabled()) 
			ret = (*it)->PostUpdate(clock->Dt());
		++it;
	}

	if (ret == UpdateReturn::UPDT_ERROR) LOG(LOG_ERROR, "Exiting with error on postupdate.");

	FinishUpdate();

	if(m_shouldClose)
	{
		// TODO: Save process

		ret = UpdateReturn::UPDT_STOP;
	}

	return ret;
}

bool App::CleanUp()
{
	bool ret = true;

	for (auto it = m_modules.rbegin(); it != m_modules.rend() && ret; ++it)
	{
		if((*it)->m_configuration & M_CLEAN_UP) 
			ret = (*it)->CleanUp();
	}

	return ret;
}

void App::OnResize(uint w, uint h)
{
	for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		if ((*it)->IsEnabled() && (*it)->m_configuration & M_RESIZE_EVENT)
		{
			(*it)->OnResize(w, h);
		}
	}
}

void App::DrawDebug()
{
	for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		if ((*it)->IsEnabled() && (*it)->m_configuration & M_DRAW_DEBUG)
		{
			(*it)->DrawDebug();
		}
	}
}

uint App::GetMaxFPS() const
{
	if (m_cappedMs > 0) return (uint)((1.0f / (float)m_cappedMs) * 1000.0f);
	
	return 0;
}

void App::SetMaxFPS(uint _fps)
{
	if (_fps > 0)
		m_cappedMs = 1000 / _fps;
	else
		m_cappedMs = 0;
}

void App::PrepareUpdate()
{
	clock->OnPrepareUpdate();
}

void App::FinishUpdate()
{
	clock->OnFinishUpdate();

	if (m_cappedMs > 0 && clock->LastFrameMs() < m_cappedMs) SDL_Delay(m_cappedMs - clock->LastFrameMs());
}
