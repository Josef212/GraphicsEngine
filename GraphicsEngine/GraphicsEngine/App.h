#ifndef __APP_H__
#define __APP_H__

#include "Defs.h"
#include "Module.h"
#include "Clock.h"

#include <vector>

//TMP
#include "TestScene.h"

class M_FileSystem;
class M_Window;
class M_Input;
class M_Editor;

class M_Render3D;

class App
{
public:
	App();
	virtual ~App();

	bool Init();
	UpdateReturn Update();
	bool CleanUp();

	void OnResize(uint w, uint h);
	void DrawDebug();

	// ===================================

	void Close() { shouldClose = true; }

	uint GetMaxFPS()const;
	void SetMaxFPS(uint _fps);

private:
	void PrepareUpdate();
	void FinishUpdate();

public:
	M_FileSystem* fs	= nullptr;
	M_Window* window	= nullptr;
	M_Input* input		= nullptr;
	M_Editor* editor	= nullptr;

	TestScene* testScene = nullptr;

	M_Render3D* render	= nullptr;

	bool drawDebug = false;

	GE::Clock* clock = nullptr;

private:
	std::vector<Module*> modules;

	bool shouldClose = false;
	uint32 cappedMs = 0;
};

extern App* app;

#endif