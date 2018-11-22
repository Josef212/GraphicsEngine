#ifndef __M_RENDER3D_H__
#define __M_RENDER3D_H__

#include "Module.h"
#include <SDL.h>

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

private:

public:
	bool showGrid = true;

private:
	SDL_GLContext context;
	bool vsync;
};

#endif // !__M_RENDER3D_H__