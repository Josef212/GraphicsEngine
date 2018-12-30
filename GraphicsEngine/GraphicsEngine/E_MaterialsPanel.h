#ifndef __E_MATERIALSPANEL_H__
#define __E_MATERIALSPANEL_H__

#include "Panel.h"
#include "IEventListener.h"

class R_Material;

class E_MaterialsPanel : public Panel, IEventListener
{
public:
	E_MaterialsPanel();
	virtual ~E_MaterialsPanel();

	EventType GetSupportedEvents() override;
	void OnEventRecieved(Event e) override;

	void OnInit() override;
	void OnDisable() override;

	void Display() override;

private:
	void EditMaterial();
	void CreateMaterial();

private:
	R_Material* m_editingMaterial = nullptr;


};

#endif // !__E_MATERIALSPANEL_H__