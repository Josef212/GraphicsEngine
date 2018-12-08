#include "m_Window.h"

#include <SDL.h>
#include "App.h"


M_Window::M_Window() : Module("M_Window", true)
{
	LOG_CREATION(moduleName.c_str());

	width = DEFAULT_WIN_WIDTH;
	height = DEFAULT_WIN_HEIGHT;
	scale = 1;

	configuration = M_INIT | M_CLEAN_UP | M_SAVE_CONFIG | M_RESIZE_EVENT;
}

M_Window::~M_Window()
{
	LOG(LOG_INFO, "\t%s: Destruction --------------", moduleName.c_str());
}

bool M_Window::Init()
{
	LOG_INIT(moduleName.c_str());

	bool ret = false;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG(LOG_ERROR, "SDL_Video could not initialize. SDL_Error: %s", SDL_GetError());
	}
	else
	{
		// TODO: Get the config 

		uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;// | SDL_WINDOW_MAXIMIZED;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		window = SDL_CreateWindow("Graphics engine.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (!window)
		{
			LOG(LOG_ERROR, "Could not create window. SDL_Error: %s", SDL_GetError());
		}
		else
		{
			ret = true;
		}
	}

	return ret;
}

bool M_Window::CleanUp()
{
	LOG_CLEANUP(moduleName.c_str());

	if (window) SDL_DestroyWindow(window);
	SDL_Quit();

	return true;
}

// ================================

void M_Window::OnResize(uint w, uint h)
{
	width = w;
	height = h;
}

// ================================

void M_Window::SetWinWidth(int w)
{
	if (w > 0)
	{
		this->width = w;
		SDL_SetWindowSize(window, width, height);
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
		this->height = h;
		SDL_SetWindowSize(window, width, height);
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
		this->width = w;
		this->height = h;
		SDL_SetWindowSize(window, width, height);
	}
	else
	{
		LOG(LOG_WARN, "Can't set window size less than 0.");
	}
}

void M_Window::SetWinScale(int s)
{
	if (s > 0)
		this->scale = s;
	else
		LOG(LOG_WARN, "Can't set window scale to 0 or less.");
}

// ================================

float M_Window::GetWinBrightness() const
{
	return SDL_GetWindowBrightness(this->window);
}

bool M_Window::SetWinBrightness(float bright)
{
	bright = CLAMP01(bright);

	if (window)
	{
		return SDL_SetWindowBrightness(this->window, bright) == 0;
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
	if (this->window) return SDL_GetWindowTitle(this->window);

	return nullptr;
}

void M_Window::SetWinTitle(const char * title)
{
	if (title && window) SDL_SetWindowTitle(this->window, title);
}

// ================================

bool M_Window::IsFullscreen() const
{
	return this->windowConfig & WC_FULLSCREEN;
}

bool M_Window::IsResizable() const
{
	return this->windowConfig & WC_RESIZABLE;
}

bool M_Window::IsBorderless() const
{
	return this->windowConfig & WC_BORDERLESS;
}

bool M_Window::IsFullscreenDesktop() const
{
	return this->windowConfig & WC_FULLSCREEN_DESKTOP;
}

void M_Window::SetFullscreen(bool set)
{
	if (window)
	{
		if (set != IsFullscreen())
		{
			if (set)
			{
				this->windowConfig |= WC_FULLSCREEN;
				this->windowConfig &= ~WC_FULLSCREEN_DESKTOP;

				if (SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN) != 0)
					LOG(LOG_WARN, "Could not set window to fullscreen: %s\n.", SDL_GetError());
			}
			else
			{
				this->windowConfig &= ~WC_FULLSCREEN;

				if (SDL_SetWindowFullscreen(this->window, 0) != 0)
					LOG(LOG_WARN, "Could not set window to windowed: %s\n.", SDL_GetError());
			}
		}
	}
}

void M_Window::SetResizable(bool set)
{
	if (set)
	{
		this->windowConfig |= WC_RESIZABLE;
	}
	else
	{
		this->windowConfig &= ~WC_RESIZABLE;
	}
}

void M_Window::SetBorderless(bool set)
{
	if (set != IsBorderless() && !IsFullscreen() && !IsFullscreenDesktop())
	{
		if (set)
		{
			this->windowConfig |= WC_BORDERLESS;
		}
		else
		{
			this->windowConfig &= ~WC_BORDERLESS;
		}

		SDL_SetWindowBordered(window, (SDL_bool)!set);
	}
}

void M_Window::SetFullscreenDesktop(bool set)
{
	if (set != IsFullscreenDesktop())
	{
		if (set)
		{
			this->windowConfig |= WC_FULLSCREEN_DESKTOP;
			this->windowConfig &= ~WC_FULLSCREEN;

			if (SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				LOG(LOG_WARN, "Could not set window to fullscreen desktop: %s\n.", SDL_GetError());
		}
		else
		{
			this->windowConfig &= ~WC_FULLSCREEN_DESKTOP;

			if (SDL_SetWindowFullscreen(this->window, 0) != 0)
				LOG(LOG_WARN, "Could not set window to windowed: %s\n.", SDL_GetError());
		}
	}
}

// ================================

void M_Window::HideWindow()
{
	if (window)SDL_HideWindow(window);
}

void M_Window::MaximizeWindow()
{
	if (window)SDL_MaximizeWindow(window);
}

void M_Window::MinimizaWindow()
{
	if (window)SDL_MinimizeWindow(window);
}

void M_Window::RestoreWindow()
{
	if (window)SDL_RestoreWindow(window);
}

void M_Window::ShowWindow()
{
	if (window)SDL_ShowWindow(window);
}

void M_Window::RaiseWindow()
{
	if (window)SDL_RaiseWindow(window);
}

// ================================

