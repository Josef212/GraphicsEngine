#ifndef __PANEL_H__
#define __PANEL_H__

#include <string>

#include "ThirdParty/ImGui/imgui.h"
#include "Resource.h"
#include <vector>


#include "App.h"
#include "M_ResourceManager.h"
#include "R_Texture.h"

class R_Shader;

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

	template<typename T>
	static T ResourcePopup(ResourceType type, const char* popupName)
	{
		T ret = nullptr;

		if (ImGui::BeginPopup(popupName))
		{
			std::vector<Resource*> res;
			app->resourceManager->GatherResourcesOfType(type, res);

			for (auto it : res)
			{
				if (type == RES_TEXTURE)
				{
					ImGui::Image((ImTextureID)static_cast<R_Texture*>(it)->TextureID(), ImVec2(16, 16));
					ImGui::SameLine();
				}

				if (ImGui::Selectable(it->GetNameCStr()))
				{
					ret = static_cast<T>(it);
					break;
				}
			}

			ImGui::EndPopup();
		}

		return ret;
	}

	static void ShaderInfo(R_Shader* shader);
	static R_Shader* ChangeShaderMenu(bool sameLine = true);

public:
	std::string m_name = "No name";

protected:
	bool m_show = false;

};

#endif // !__PANEL_H__
