#ifndef __M_RENDER3D_H__
#define __M_RENDER3D_H__

#include "Module.h"
#include <SDL.h>

class R_Scene;

class M_Render3D : public Module
{
public:
	M_Render3D();
	virtual ~M_Render3D();

	bool Init() override;
	UpdateReturn PreUpdate(float dt) override;
	UpdateReturn Update(float dt) override;
	UpdateReturn PostUpdate(float dt) override;
	bool CleanUp() override;

	void OnResize(uint w, uint h) override;

	// ==============

	bool GetVSync()const { return vsync; }
	void SetVSync(bool set);

	// -------------------------

	void SetActiveScene(R_Scene* sc);
	R_Scene* GetActiveScene()const { return activeScene; }

private:

public:
	bool showGrid = true;
	SDL_GLContext context;

private:
	bool vsync;

	R_Scene* activeScene = nullptr;
};

#endif // !__M_RENDER3D_H__