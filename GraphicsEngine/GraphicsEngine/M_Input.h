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
	KeyState GetKey(int id)const { return m_keyboard[id]; }
	KeyState GetMouseButton(int id)const { return m_mouse[id]; }

	bool GetKeyDown(int id)const { return m_keyboard[id] == KEY_DOWN; }
	bool GetKeyUp(int id)const { return m_keyboard[id] == KEY_UP; }
	bool GetKeyRepeat(int id)const { return m_keyboard[id] == KEY_REPEAT; }

	bool GetMouseDown(int id)const { return m_mouse[id] == KEY_DOWN; }
	bool GetMouseUp(int id)const { return m_mouse[id] == KEY_UP; }
	bool GetMouseRepeat(int id)const { return m_mouse[id] == KEY_REPEAT; }

	int GetMouseX()const { return m_mouseX; }
	int GetMouseY()const { return m_mouseY; }
	void GetMousePos(int& x, int& y)const { x = this->m_mouseX; y = this->m_mouseY; }
	int GetMouseMotionX()const { return m_mouseMotionX; }
	int GetMouseMotionY()const { return m_mouseMotionY; }
	void GetMouseMotion(int& mx, int& my)const { mx = this->m_mouseMotionX; my = this->m_mouseMotionY; }
	int GetMouseWheel()const { return m_wheelY; }

private:

public:

private:
	KeyState * m_keyboard = nullptr;
	KeyState m_mouse[MAX_MOUSE_BUTTONS];
	int m_mouseX, m_mouseY, m_mouseMotionX, m_mouseMotionY, m_wheelY;
};

#endif