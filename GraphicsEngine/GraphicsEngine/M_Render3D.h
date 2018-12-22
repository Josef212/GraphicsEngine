#ifndef __M_RENDER3D_H__
#define __M_RENDER3D_H__

#include "Module.h"
#include "IEventListener.h"
#include <SDL.h>

class R_Scene;
class R_Renderer;

class M_Render3D : public Module, IEventListener
{
public:
	M_Render3D();
	virtual ~M_Render3D();

	bool Init() override;
	bool Start() override;
	UpdateReturn PreUpdate(float dt) override;
	UpdateReturn Update(float dt) override;
	UpdateReturn PostUpdate(float dt) override;
	bool CleanUp() override;

	EventType GetSupportedEvents()override;
	void OnEventRecieved(Event e)override;

	// ==============

	bool GetVSync()const { return m_vsync; }
	void SetVSync(bool set);

	// -------------------------

	R_Renderer* GetActiveRenderer() { return m_activeRenderer; }
	void SetActiveRenderer(R_Renderer* renderer) { m_activeRenderer = renderer; }
	

private:

public:
	bool m_showGrid = true;
	SDL_GLContext m_context;

private:
	bool m_vsync;

	R_Scene* m_activeScene			= nullptr;
	R_Renderer* m_activeRenderer		= nullptr;
};

#endif // !__M_RENDER3D_H__