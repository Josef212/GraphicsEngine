#ifndef __PANEL_H__
#define __PANEL_H__

#include <string>

#include "ThirdParty/ImGui/imgui.h"

class Panel
{
public:
	Panel(const char* name) { this->name = name; }
	virtual ~Panel() = default;

	virtual void OnInit() {}
	virtual void Display() = 0;

	void Enable() { show = true; OnInit(); }
	void Disable() { show = false; }
	void SwapShow() { show ? Disable() : Enable(); }

	bool Visible()const { return show; }
	
public:
	std::string name = "No name";

protected:
	bool show = false;

};

#endif // !__PANEL_H__