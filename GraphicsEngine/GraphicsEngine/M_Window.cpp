#include "m_Window.h"

#include "EventManager.h"

#include <SDL.h>
#include "App.h"


M_Window::M_Window() : Module("M_Window", true)
{
	LOG_CREATION(m_moduleName.c_str());

	m_width = DEFAULT_WIN_WIDTH;
	m_height = DEFAULT_WIN_HEIGHT;
	m_scale = 1;

	m_configuration = M_INIT | M_CLEAN_UP | M_SAVE_CONFIG;
}

M_Window::~M_Window()
{
	LOG(LOG_INFO, "\t%s: Destruction --------------", m_moduleName.c_str());
}

bool M_Window::Init()
{
	LOG_INIT(m_moduleName.c_str());

	app->eventManager->AddEventListener(this);

	bool ret = false;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG(LOG_ERROR, "SDL_Video could not initialize. SDL_Error: %s", SDL_GetError());
	}
	else
	{
		// TODO: Get the config 

		uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		m_window = SDL_CreateWindow("Graphics engine.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, flags);

		if (!m_window)
		{
			LOG(LOG_ERROR, "Could not create window. SDL_Error: %s", SDL_GetError());
		}
		else
		{
			SDL_GetWindowSize(m_window, (int*)&m_width, (int*)&m_height);
			ret = true;
		}
	}

	return ret;
}

bool M_Window::CleanUp()
{
	LOG_CLEANUP(m_moduleName.c_str());

	app->eventManager->RemoveEventListener(this);

	if (m_window) SDL_DestroyWindow(m_window);
	SDL_Quit();

	return true;
}

// ================================

EventType M_Window::GetSupportedEvents()
{
	return EventType::EVENT_WINDOW_RESIZE;
}

void M_Window::OnEventRecieved(Event e)
{
	if (e.type == EventType::EVENT_WINDOW_RESIZE)
	{
		m_width = e.data._v2.x;
		m_height = e.data._v2.y;

	}
}

// ================================

void M_Window::SetWinWidth(int w)
{
	if (w > 0)
	{
		this->m_width = w;
		SDL_SetWindowSize(m_window, m_width, m_height);
	}
	else
	{
		LOG(LOG_WARN, "Can't set window width less than 0.");
	}
}

void M_Window::SetWinHeight(int h)
{
	if (h > 0)
	{
		this->m_height = h;
		SDL_SetWindowSize(m_window, m_width, m_height);
	}
	else
	{
		LOG(LOG_WARN, "Can't set window height less than 0.");
	}
}

void M_Window::SetWinSize(int w, int h)
{
	if (w > 0 && h > 0)
	{
		this->m_width = w;
		this->m_height = h;
		SDL_SetWindowSize(m_window, m_width, m_height);
	}
	else
	{
		LOG(LOG_WARN, "Can't set window size less than 0.");
	}
}

void M_Window::SetWinScale(int s)
{
	if (s > 0)
		this->m_scale = s;
	else
		LOG(LOG_WARN, "Can't set window scale to 0 or less.");
}

// ================================

float M_Window::GetWinBrightness() const
{
	return SDL_GetWindowBrightness(this->m_window);
}

bool M_Window::SetWinBrightness(float bright)
{
	bright = CLAMP01(bright);

	if (m_window)
	{
		return SDL_SetWindowBrightness(this->m_window, bright) == 0;
	}

	return false;
}

// ================================

void M_Window::GetWinRange(int & minW, int & minH, int & maxW, int & maxH) const
{
	minW = 640;
	minH = 480;
	maxW = 3000;
	maxH = 2000;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		LOG(LOG_WARN, "SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else
	{
		maxW = dm.w;
		maxH = dm.h;
	}
}

int M_Window::GetWinRefresh() const
{
	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		LOG(LOG_WARN, "SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return 0;
	}
	else
	{
		return dm.refresh_rate;
	}
}

// ================================

const char * M_Window::GetWinTitle() const
{
	if (this->m_window) return SDL_GetWindowTitle(this->m_window);

	return nullptr;
}

void M_Window::SetWinTitle(const char * title)
{
	if (title && m_window) SDL_SetWindowTitle(this->m_window, title);
}

// ================================

bool M_Window::IsFullscreen() const
{
	return this->m_windowConfig & WC_FULLSCREEN;
}

bool M_Window::IsResizable() const
{
	return this->m_windowConfig & WC_RESIZABLE;
}

bool M_Window::IsBorderless() const
{
	return this->m_windowConfig & WC_BORDERLESS;
}

bool M_Window::IsFullscreenDesktop() const
{
	return this->m_windowConfig & WC_FULLSCREEN_DESKTOP;
}

void M_Window::SetFullscreen(bool set)
{
	if (m_window)
	{
		if (set != IsFullscreen())
		{
			if (set)
			{
				this->m_windowConfig |= WC_FULLSCREEN;
				this->m_windowConfig &= ~WC_FULLSCREEN_DESKTOP;

				if (SDL_SetWindowFullscreen(this->m_window, SDL_WINDOW_FULLSCREEN) != 0)
					LOG(LOG_WARN, "Could not set window to fullscreen: %s\n.", SDL_GetError());
			}
			else
			{
				this->m_windowConfig &= ~WC_FULLSCREEN;

				if (SDL_SetWindowFullscreen(this->m_window, 0) != 0)
					LOG(LOG_WARN, "Could not set window to windowed: %s\n.", SDL_GetError());
			}
		}
	}
}

void M_Window::SetResizable(bool set)
{
	if (set)
	{
		this->m_windowConfig |= WC_RESIZABLE;
	}
	else
	{
		this->m_windowConfig &= ~WC_RESIZABLE;
	}
}

void M_Window::SetBorderless(bool set)
{
	if (set != IsBorderless() && !IsFullscreen() && !IsFullscreenDesktop())
	{
		if (set)
		{
			this->m_windowConfig |= WC_BORDERLESS;
		}
		else
		{
			this->m_windowConfig &= ~WC_BORDERLESS;
		}

		SDL_SetWindowBordered(m_window, (SDL_bool)!set);
	}
}

void M_Window::SetFullscreenDesktop(bool set)
{
	if (set != IsFullscreenDesktop())
	{
		if (set)
		{
			this->m_windowConfig |= WC_FULLSCREEN_DESKTOP;
			this->m_windowConfig &= ~WC_FULLSCREEN;

			if (SDL_SetWindowFullscreen(this->m_window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				LOG(LOG_WARN, "Could not set window to fullscreen desktop: %s\n.", SDL_GetError());
		}
		else
		{
			this->m_windowConfig &= ~WC_FULLSCREEN_DESKTOP;

			if (SDL_SetWindowFullscreen(this->m_window, 0) != 0)
				LOG(LOG_WARN, "Could not set window to windowed: %s\n.", SDL_GetError());
		}
	}
}

// ================================

void M_Window::HideWindow()
{
	if (m_window)SDL_HideWindow(m_window);
}

void M_Window::MaximizeWindow()
{
	if (m_window)SDL_MaximizeWindow(m_window);
}

void M_Window::MinimizaWindow()
{
	if (m_window)SDL_MinimizeWindow(m_window);
}

void M_Window::RestoreWindow()
{
	if (m_window)SDL_RestoreWindow(m_window);
}

void M_Window::ShowWindow()
{
	if (m_window)SDL_ShowWindow(m_window);
}

void M_Window::RaiseWindow()
{
	if (m_window)SDL_RaiseWindow(m_window);
}

// ================================

