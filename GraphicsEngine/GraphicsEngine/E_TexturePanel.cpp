#include "E_TexturePanel.h"

#include "R_Texture.h"
#include "App.h"
#include "M_ResourceManager.h"

E_TexturePanel::E_TexturePanel() : Panel("E_TexturePanel")
{
}


E_TexturePanel::~E_TexturePanel()
{
}

void E_TexturePanel::Display()
{
	ImGui::Begin(name.c_str(), &show);
	{
		TexturePanelHeader();

		std::vector<Resource*> texs;
		app->resourceManager->GatherResourcesOfType(RES_TEXTURE, texs);

		static int sT = -1;
		for(int i = 0; i < texs.size(); ++i)
		{
			R_Texture* tex = static_cast<R_Texture*>(texs[i]);
			if(tex)
			{
				ImGuiTreeNodeFlags flags = 0;

				if(ImGui::TreeNodeEx(tex->GetNameCStr(), flags))
				{
					if (ImGui::IsItemClicked()) sT = i;

					TextureInfo(tex);

					ImGui::TreePop();
				}
			}

			if(i < texs.size() - 1)
			{
				ImGui::Separator();
				ImGui::Separator();
			}
		}

		ImGui::End();
	}
}

void E_TexturePanel::TextureInfo(R_Texture* tex)
{
	ImGui::Text("Path: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), tex->Path());

	ImGui::Text("Size: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d x %d", tex->Width(), tex->Height());

	ImGui::Text("Num channels: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", tex->NumChannels());

	ImGui::Text("Texture ID: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", tex->TextureID());

	ImGui::Image((ImTextureID)tex->TextureID(), ImVec2(200, 200));
}

void E_TexturePanel::TexturePanelHeader()
{
	if (ImGui::Button("Load new texture")) ImGui::OpenPopup("Load texture");
	if(ImGui::BeginPopupModal("Load texture"))
	{
		// TODO: File dialogue

		if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}
