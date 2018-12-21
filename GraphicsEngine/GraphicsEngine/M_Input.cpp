#include "M_Input.h"

#include <SDL.h>

#include "App.h"
#include "M_Window.h"
#include "M_Editor.h"


M_Input::M_Input() : Module("M_Input", true)
{
	LOG_CREATION(m_moduleName.c_str());

	m_keyboard = new KeyState[MAX_KEYS];
	memset(m_keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(m_mouse, KEY_IDLE, sizeof(KeyState) * MAX_MOUSE_BUTTONS);

	m_configuration = M_INIT | M_PRE_UPDATE | M_CLEAN_UP;
}


M_Input::~M_Input()
{
	LOG_DESTRUCTION(m_moduleName.c_str());

	RELEASE_ARRAY(m_keyboard);
}

bool M_Input::Init()
{
	bool ret = true;

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG(LOG_ERROR, "Could not init sdl events! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

UpdateReturn M_Input::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (m_keyboard[i] == KEY_IDLE)
				m_keyboard[i] = KEY_DOWN;
			else
				m_keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (m_keyboard[i] == KEY_REPEAT || m_keyboard[i] == KEY_DOWN)
				m_keyboard[i] = KEY_UP;
			else
				m_keyboard[i] = KEY_IDLE;
		}
	}

	uint winScl = app->window->GetWinScale();
	Uint32 buttons = SDL_GetMouseState(&m_mouseX, &m_mouseY);

	m_mouseX /= winScl;
	m_mouseY /= winScl;
	m_wheelY = 0;
	m_mouseMotionX = m_mouseMotionY = 0;

	for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (m_mouse[i] == KEY_IDLE)
				m_mouse[i] = KEY_DOWN;
			else
				m_mouse[i] = KEY_REPEAT;
		}
		else
		{
			if (m_mouse[i] == KEY_REPEAT || m_mouse[i] == KEY_DOWN)
				m_mouse[i] = KEY_UP;
			else
				m_mouse[i] = KEY_IDLE;
		}
	}

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		app->editor->PassInput(&e);

		switch (e.type)
		{
		case SDL_MOUSEWHEEL:
			m_wheelY = e.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			m_mouseX = e.motion.x / winScl;
			m_mouseY = e.motion.y / winScl;
			m_mouseMotionX = e.motion.xrel / winScl;
			m_mouseMotionY = e.motion.yrel / winScl;
			break;

		case SDL_QUIT:
			app->Close();
			break;

		case SDL_WINDOWEVENT:
		{
			switch (e.window.type)
			{
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				//TODO:Send event 
				app->OnResize(e.window.data1, e.window.data2);

				break;
			}
		}
		break;

		case SDL_DROPFILE:
		{
			char* droppedFile = e.drop.file;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped on window: ",
				droppedFile, app->window->GetWindow());
			//app->resources->ImportFile(dropped_file);	//TODO: Should do this by events
			SDL_free(droppedFile);
		}
		break;

		}

	}

	SDL_PollEvent(&e);

	return UPDT_CONTINUE;
}

bool M_Input::CleanUp()
{
	LOG_CLEANUP(m_moduleName.c_str());

	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}
