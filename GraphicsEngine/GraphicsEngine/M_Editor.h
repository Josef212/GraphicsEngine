#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Module.h"
#include <vector>

union SDL_Event;

class Panel;
class E_GeometryPanel;
class E_TexturePanel;

class M_Editor : public Module
{
public:
	M_Editor();
	virtual ~M_Editor();

	bool Init() override;
	bool Start() override;
	UpdateReturn PreUpdate(float dt) override;
	UpdateReturn Update(float dt) override;
	bool CleanUp() override;

	void Render();
	void PassInput(SDL_Event* ev);

	bool UsingMouse()const { return isUsingMouse; }
	bool UsingKeyboard()const { return isUsingKeyboard; }

public:
	E_GeometryPanel* geoPanel = nullptr;
	E_TexturePanel* texPanel = nullptr;

private:
	bool isUsingMouse = false, isUsingKeyboard = false;
	bool showImGuiDemo = false;

	std::vector<Panel*> panels;

};

#endif // !__EDITOR_H__