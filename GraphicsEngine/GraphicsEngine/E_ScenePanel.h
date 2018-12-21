#ifndef __E_SCENEPANEL_H__
#define __E_SCENEPANEL_H__

#include "Panel.h"

class R_Scene;

class E_ScenePanel : public Panel
{
public:
	E_ScenePanel();
	virtual ~E_ScenePanel();

	void Display() override;

private:
	void CreateSceneMenu();
	void SceneInfo(R_Scene* sc);
	void ActiveSceneInfo(R_Scene* sc);
};

#endif // !__E_SCENEPANEL_H__