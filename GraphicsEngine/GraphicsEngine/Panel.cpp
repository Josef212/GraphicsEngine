#include "Panel.h"

#include "App.h"
#include "M_ResourceManager.h"

Resource * Panel::ResourcePopup(ResourceType type)
{
	std::vector<Resource*> res;
	app->resourceManager->GatherResourcesOfType(type, res);

	for(auto it : res)
	{
		if (ImGui::Selectable(it->GetNameCStr()))
			return it;
	}

	return nullptr;
}
