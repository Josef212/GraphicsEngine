#ifndef __M_RENDER3D_H__
#define __M_RENDER3D_H__

#include "Module.h"
#include <SDL.h>

class R_Scene;
class R_Renderer;

class M_Render3D : public Module
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

	void OnResize(uint w, uint h) override;

	// ==============

	bool GetVSync()const { return vsync; }
	void SetVSync(bool set);

	// -------------------------

	R_Renderer* GetActiveRenderer() { return activeRenderer; }
	void SetActiveRenderer(R_Renderer* renderer) { activeRenderer = renderer; }
	

private:

public:
	bool showGrid = true;
	SDL_GLContext context;

private:
	bool vsync;

	R_Scene* activeScene			= nullptr;
	R_Renderer* activeRenderer		= nullptr;
};

#endif // !__M_RENDER3D_H__