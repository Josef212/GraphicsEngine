#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Module.h"

union SDL_Event;

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

private:
	bool isUsingMouse = false, isUsingKeyboard = false;
	bool showImGuiDemo = false;

};

#endif // !__EDITOR_H__