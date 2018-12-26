#ifndef __E_INSPECTORPANEL_H__
#define __E_INSPECTORPANEL_H__

#include "Panel.h"

class R_Model;
class Light;

class E_InspectorPanel : public Panel
{
public:
	E_InspectorPanel();
	virtual ~E_InspectorPanel();

	void OnInit() override;
	void Display() override;

private:
	void ModelDisplay(R_Model* model);
	void LightDisplay(Light* light, int count);
};

#endif // !__E_INSPECTORPANEL_H__