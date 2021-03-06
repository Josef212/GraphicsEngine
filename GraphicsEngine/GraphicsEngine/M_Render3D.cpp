#include "M_Render3D.h"

#include "App.h"
#include "M_Window.h"
#include "M_Editor.h"

#include "OpenGL.h"

#include "EventManager.h"
#include "M_ResourceManager.h"
#include "M_SceneManager.h"
#include "R_Scene.h"
#include "R_Renderer.h"

#include "R_ForwardRenderer.h" // TMP
#include "R_DeferredRenderer.h"


M_Render3D::M_Render3D() : Module("M_Module3D", true), IEventListener()
{
	LOG_CREATION(m_moduleName.c_str());

	m_vsync = false;

	m_configuration = M_INIT | M_START | M_PRE_UPDATE | M_POST_UPDATE | M_CLEAN_UP | M_SAVE_CONFIG;
}


M_Render3D::~M_Render3D()
{
	LOG_DESTRUCTION(m_moduleName.c_str());
}

bool M_Render3D::Init()
{
	bool ret = true;

	app->eventManager->AddEventListener(this);

	m_context = SDL_GL_CreateContext(app->window->GetWindow());

	if(!m_context)
	{
		LOG(LOG_ERROR, "OpenGL could not create context. SDL_Error: %s", SDL_GetError());
		ret = false;
	}
	else
	{
		glewExperimental = GL_TRUE;
		GLenum gl = glewInit();

		if(gl != GLEW_OK)
		{
			LOG(LOG_ERROR, "GLEW could not init: %s", glewGetErrorString(gl));
			ret = false;
		}
		else
		{
			gl = glGetError();
			if(gl != GL_NO_ERROR)
			{
				LOG(LOG_ERROR, "Could not init OpenGL! %s\n", glewGetErrorString(gl));
				ret = false;
			}
		}
	}

	if(ret)
	{
		LOG(LOG_INFO, "Vendor: %s", glGetString(GL_VENDOR));
		LOG(LOG_INFO, "Renderer: %s", glGetString(GL_RENDERER));
		LOG(LOG_INFO, "OpenGL version supported %s", glGetString(GL_VERSION));
		LOG(LOG_INFO, "GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		SetVSync(m_vsync);

		glClearDepth(1.f);
		glClearColor(0.f, 0.f, 0.f, 1.f);

		glEnable(GL_DEPTH_TEST);// | GL_CULL_FACE); //TODO: Must check how cull face
		//glCullFace(GL_FRONT_AND_BACK);
		glDepthFunc(GL_LESS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	return ret;
}

bool M_Render3D::Start()
{
	m_activeRenderer = new R_ForwardRenderer("Forward renderer");
	//m_activeRenderer = new R_DeferredRenderer("R_DeferredRenderer");

	Event ev;
	ev.type = EventType::EVENT_WINDOW_RESIZE;
	ev.data._v2.x = app->window->GetWinWidth();
	ev.data._v2.y = app->window->GetWinHeight();
	app->eventManager->FireEvent(ev);

	return true;
}

UpdateReturn M_Render3D::PreUpdate(float dt)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UpdateReturn::UPDT_CONTINUE;
}

UpdateReturn M_Render3D::Update(float dt)
{
	return UpdateReturn::UPDT_CONTINUE;
}

UpdateReturn M_Render3D::PostUpdate(float dt)
{
	// TODO: Can improve this a lot but ok for now
	m_activeScene = app->sceneManager->GetActiveScene();

	if (m_activeScene && m_activeRenderer)
	{
		m_activeRenderer->RenderScene(m_activeScene);
	}

	// TODO: Grid

	if (app->m_drawDebug) app->DrawDebug();

	app->editor->Render();

	SDL_GL_SwapWindow(app->window->GetWindow());

	return UpdateReturn::UPDT_CONTINUE;
}

bool M_Render3D::CleanUp()
{
	LOG_CLEANUP(m_moduleName.c_str());

	app->eventManager->RemoveEventListener(this);
	SDL_GL_DeleteContext(m_context);

	return true;
}

EventType M_Render3D::GetSupportedEvents()
{
	return EventType::EVENT_WINDOW_RESIZE;
}

void M_Render3D::OnEventRecieved(Event e)
{
	if (e.type == EventType::EVENT_WINDOW_RESIZE)
	{
		glViewport(0, 0, e.data._v2.x, e.data._v2.y);
	}
}

void M_Render3D::SetVSync(bool set)
{
	if (set != m_vsync)
	{
		m_vsync = set;
		if (SDL_GL_SetSwapInterval(m_vsync ? 1 : 0) < 0)
			LOG(LOG_WARN, "Unable to set VSync. SDL_Error: %s", SDL_GetError());
	}
}
