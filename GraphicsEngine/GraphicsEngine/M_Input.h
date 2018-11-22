#ifndef __M_INPUT_H__
#define __M_INPUT_H__

#include "Module.h"

#define MAX_MOUSE_BUTTONS 5
#define MAX_KEYS 300

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class M_Input : public Module
{
public:
	M_Input();
	virtual ~M_Input();

	bool Init()override;
	UpdateReturn PreUpdate(float dt)override;
	bool CleanUp()override;

	// ================================

	// Get keyboard and mouse state
	KeyState GetKey(int id)const { return keyboard[id]; }
	KeyState GetMouseButton(int id)const { return mouse[id]; }

	bool GetKeyDown(int id)const { return keyboard[id] == KEY_DOWN; }
	bool GetKeyUp(int id)const { return keyboard[id] == KEY_UP; }
	bool GetKeyRepeat(int id)const { return keyboard[id] == KEY_REPEAT; }

	bool GetMouseDown(int id)const { return mouse[id] == KEY_DOWN; }
	bool GetMouseUp(int id)const { return mouse[id] == KEY_UP; }
	bool GetMouseRepeat(int id)const { return mouse[id] == KEY_REPEAT; }

	int GetMouseX()const { return mouseX; }
	int GetMouseY()const { return mouseY; }
	void GetMousePos(int& x, int& y)const { x = this->mouseX; y = this->mouseY; }
	int GetMouseMotionX()const { return mouseMotionX; }
	int GetMouseMotionY()const { return mouseMotionY; }
	void GetMouseMotion(int& mx, int& my)const { mx = this->mouseMotionX; my = this->mouseMotionY; }
	int GetMouseWheel()const { return wheelY; }

private:

public:

private:
	KeyState * keyboard = nullptr;
	KeyState mouse[MAX_MOUSE_BUTTONS];
	int mouseX, mouseY, mouseMotionX, mouseMotionY, wheelY;
};

#endif