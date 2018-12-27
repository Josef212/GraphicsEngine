#ifndef __PANEL_H__
#define __PANEL_H__

#include <string>

#include "ThirdParty/ImGui/imgui.h"

class Panel
{
public:
	Panel(const char* name) { this->m_name = name; }
	virtual ~Panel() = default;

	virtual void OnInit() {}
	virtual void OnDisable() {}
	virtual void Display() = 0;

	void Enable() { m_show = true; OnInit(); }
	void Disable() { m_show = false; OnDisable(); }
	void SwapShow() { m_show ? Disable() : Enable(); }

	bool Visible()const { return m_show; }
	
public:
	std::string m_name = "No name";

protected:
	bool m_show = false;

};

#endif // !__PANEL_H__