#ifndef __E_INSPECTORPANEL_H__
#define __E_INSPECTORPANEL_H__

#include "Panel.h"
#include "IEventListener.h"

class R_Model;
class Light;

class E_InspectorPanel : public Panel, IEventListener
{
public:
	E_InspectorPanel();
	virtual ~E_InspectorPanel();

	void OnInit() override;
	void OnDisable() override;
	void Display() override;

	EventType GetSupportedEvents() override;
	void OnEventRecieved(Event e) override;
private:
	void ModelDisplay(R_Model* model);
	void LightDisplay(Light* light, int count);

	void EditModel();

private:
	R_Model* m_editingModel = nullptr;
};

#endif // !__E_INSPECTORPANEL_H__