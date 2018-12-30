#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Module.h"
#include "FileDialogue.h"

#include <vector>

#define FILE_MAX_SIZE 250

union SDL_Event;

class Panel;
class E_GeometryPanel;
class E_TexturePanel;
class E_ScenePanel;
class E_InspectorPanel;
class E_MaterialsPanel;

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

	bool UsingMouse()const { return m_isUsingMouse; }
	bool UsingKeyboard()const { return m_isUsingKeyboard; }

private:

public:
	E_GeometryPanel* m_geoPanel				= nullptr;
	E_TexturePanel* m_texPanel				= nullptr;
	E_ScenePanel* m_scenePanel				= nullptr;
	E_InspectorPanel* m_inspector			= nullptr;
	E_MaterialsPanel* m_materialsPanel		= nullptr;

private:
	bool m_isUsingMouse = false, m_isUsingKeyboard = false;
	bool m_showImGuiDemo = false;

	FileDialogue m_fileDialogue;

	std::vector<Panel*> m_panels;

};

#endif // !__EDITOR_H__