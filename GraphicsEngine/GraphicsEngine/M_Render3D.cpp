#include "M_Render3D.h"

#include "App.h"
#include "M_Window.h"


#include "OpenGL.h"


M_Render3D::M_Render3D() : Module("M_Module3D", true)
{
	LOG_CREATION(moduleName.c_str());

	vsync = false;

	configuration = M_INIT | M_START | M_PRE_UPDATE | M_POST_UPDATE | M_CLEAN_UP | M_SAVE_CONFIG | M_RESIZE_EVENT;
}


M_Render3D::~M_Render3D()
{
	LOG_DESTRUCTION(moduleName.c_str());
}

bool M_Render3D::Init()
{
	bool ret = true;

	context = SDL_GL_CreateContext(app->window->GetWindow());

	if(!context)
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

		SetVSync(vsync);

		glClearDepth(1.f);
		glClearColor(0.f, 0.f, 0.f, 1.f);

		glEnable(GL_DEPTH_TEST | GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		OnResize(app->window->GetWinWidth(), app->window->GetWinHeight());
	}

	return ret;
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


	// TODO: Grid

	if (app->drawDebug) app->DrawDebug();

	//TODO: Editor

	SDL_GL_SwapWindow(app->window->GetWindow());

	return UpdateReturn::UPDT_CONTINUE;
}

bool M_Render3D::CleanUp()
{
	LOG_CLEANUP(moduleName.c_str());

	SDL_GL_DeleteContext(context);

	return true;
}

void M_Render3D::OnResize(uint w, uint h)
{
	glViewport(0, 0, w, h);
}

void M_Render3D::SetVSync(bool set)
{
	if (set != vsync)
	{
		vsync = set;
		if (SDL_GL_SetSwapInterval(vsync ? 1 : 0) < 0)
			LOG(LOG_WARN, "Unable to set VSync. SDL_Error: %s", SDL_GetError());
	}
}
