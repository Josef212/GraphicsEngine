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

	clock = new GE::Clock();
	eventManager = new EventManager();

	fs = new M_FileSystem();
	window = new M_Window();
	input = new M_Input();
	editor = new M_Editor();
	resourceManager = new M_ResourceManager();
	sceneManager = new M_SceneManager();

	render = new M_Render3D();


	modules.push_back(fs);
	modules.push_back(window);
	modules.push_back(input);
	modules.push_back(editor);
	modules.push_back(resourceManager);
	modules.push_back(sceneManager);

	modules.push_back(render);
}


App::~App()
{
	for(auto it = modules.rbegin(); it != modules.rend(); ++it)
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

	auto it = modules.begin();
	while(it != modules.end() && ret)
	{
		if ((*it)->configuration & M_INIT) ret = (*it)->Init();
		++it;
	}

	if (!ret) return ret;

	LOG(LOG_INFO, "Starting modules.");

	it = modules.begin();
	while (it != modules.end() && ret)
	{
		if ((*it)->configuration & M_START) ret = (*it)->Start();
		++it;
	}

	return ret;
}

UpdateReturn App::Update()
{
	UpdateReturn ret = UpdateReturn::UPDT_CONTINUE;

	PrepareUpdate();

	auto it = modules.begin();
	while (it != modules.end() && ret == UpdateReturn::UPDT_CONTINUE)
	{
		if ((*it)->configuration & M_PRE_UPDATE && (*it)->IsEnabled()) ret = (*it)->PreUpdate(clock->Dt());
		++it;
	}

	if (ret == UpdateReturn::UPDT_ERROR) LOG(LOG_ERROR, "Exiting with error on preupdate.");

	it = modules.begin();
	while (it != modules.end() && ret == UpdateReturn::UPDT_CONTINUE)
	{
		if ((*it)->configuration & M_UPDATE && (*it)->IsEnabled()) ret = (*it)->Update(clock->Dt());
		++it;
	}

	if (ret == UpdateReturn::UPDT_ERROR) LOG(LOG_ERROR, "Exiting with error on update.");

	it = modules.begin();
	while (it != modules.end() && ret == UpdateReturn::UPDT_CONTINUE)
	{
		if ((*it)->configuration & M_POST_UPDATE && (*it)->IsEnabled()) ret = (*it)->PostUpdate(clock->Dt());
		++it;
	}

	if (ret == UpdateReturn::UPDT_ERROR) LOG(LOG_ERROR, "Exiting with error on postupdate.");

	FinishUpdate();

	if(shouldClose)
	{
		// TODO: Save process

		ret = UpdateReturn::UPDT_STOP;
	}

	return ret;
}

bool App::CleanUp()
{
	bool ret = true;

	for (auto it = modules.rbegin(); it != modules.rend() && ret; ++it)
	{
		if((*it)->configuration & M_CLEAN_UP) ret = (*it)->CleanUp();
	}

	return ret;
}

void App::OnResize(uint w, uint h)
{
	for (auto it = modules.begin(); it != modules.end(); ++it)
	{
		if ((*it)->IsEnabled() && (*it)->configuration & M_RESIZE_EVENT)
		{
			(*it)->OnResize(w, h);
		}
	}
}

void App::DrawDebug()
{
	for (auto it = modules.begin(); it != modules.end(); ++it)
	{
		if ((*it)->IsEnabled() && (*it)->configuration & M_DRAW_DEBUG)
		{
			(*it)->DrawDebug();
		}
	}
}

uint App::GetMaxFPS() const
{
	if (cappedMs > 0) return (uint)((1.0f / (float)cappedMs) * 1000.0f);
	
	return 0;
}

void App::SetMaxFPS(uint _fps)
{
	if (_fps > 0)
		cappedMs = 1000 / _fps;
	else
		cappedMs = 0;
}

void App::PrepareUpdate()
{
	clock->OnPrepareUpdate();
}

void App::FinishUpdate()
{
	clock->OnFinishUpdate();

	if (cappedMs > 0 && clock->LastFrameMs() < cappedMs) SDL_Delay(cappedMs - clock->LastFrameMs());
}
