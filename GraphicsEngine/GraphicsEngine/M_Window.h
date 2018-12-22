#ifndef __M_WINDOW_H__
#define __M_WINDOW_H__

#include "Module.h"
#include "IEventListener.h"

struct SDL_Window;

typedef enum
{
	WC_NONE = 0,
	WC_FULLSCREEN = 1 << 0,
	WC_RESIZABLE = 1 << 1,
	WC_BORDERLESS = 1 << 2,
	WC_FULLSCREEN_DESKTOP = 1 << 3

} WindowConfig;

class M_Window : public Module, IEventListener
{
public:
	M_Window();
	virtual ~M_Window();

	bool Init() override;
	bool CleanUp() override;

	EventType GetSupportedEvents()override;
	void OnEventRecieved(Event e)override;

	// ================================

	SDL_Window* GetWindow()const { return m_window; }

	// ================================

	int GetWinWidth()const { return m_width; }
	int GetWinHeight()const { return m_height; }
	void GetWinSize(int& width, int& height)const { width = this->m_width, height = this->m_height; }
	uint GetWinScale()const { return m_scale; }

	void SetWinWidth(int w);
	void SetWinHeight(int h);
	void SetWinSize(int w, int h);
	void SetWinScale(int s);

	// ================================

	float GetWinBrightness()const;

	bool SetWinBrightness(float bright);

	// ================================

	void GetWinRange(int& minW, int& minH, int& maxW, int& maxH)const;
	int GetWinRefresh()const;

	// ================================

	const char* GetWinTitle()const;

	void SetWinTitle(const char* title);

	// ================================

	bool IsFullscreen()const;
	bool IsResizable()const;
	bool IsBorderless()const;
	bool IsFullscreenDesktop()const;

	void SetFullscreen(bool set);
	void SetResizable(bool set);
	void SetBorderless(bool set);
	void SetFullscreenDesktop(bool set);

	// ================================

	void HideWindow();
	void MaximizeWindow();
	void MinimizaWindow();
	void RestoreWindow();
	void ShowWindow();
	void RaiseWindow();

	// ================================

private:
	uint m_width, m_height, m_scale;
	SDL_Window* m_window = nullptr;

	WORD m_windowConfig = 0;
};

#endif // !__M_WINDOW_H__