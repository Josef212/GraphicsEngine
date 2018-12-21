#include "E_ScenePanel.h"

#include "R_Scene.h"
#include "App.h"
#include "M_ResourceManager.h"
#include "M_SceneManager.h"

E_ScenePanel::E_ScenePanel() : Panel("E_ScenePanel")
{
}


E_ScenePanel::~E_ScenePanel()
{
}

void E_ScenePanel::Display()
{
	ImGui::Begin(name.c_str(), &show);
	{
		// Create scene


		// ---------------

		std::vector<Resource*> scenes;
		app->resourceManager->GatherResourcesOfType(RES_SCENE, scenes);
		R_Scene* activeScene = app->sceneManager->GetActiveScene();

		// Active scene inspector

		if (activeScene)
		{
			ActiveSceneInfo(activeScene);
		}

		// -----------------------

		// All scenes display

		static int sS = -1;
		for (int i = 0; i < scenes.size(); ++i)
		{
			ImGuiTreeNodeFlags flags = 0;

			R_Scene* scene = static_cast<R_Scene*>(scenes[i]);
			if (scene)
			{
				if (ImGui::TreeNodeEx(scene->GetNameCStr(), flags))
				{
					if (ImGui::IsItemClicked()) sS = i;

					SceneInfo(scene);

					ImGui::TreePop();
				}

				if (i < scenes.size() - 1)
				{
					ImGui::Separator();
					ImGui::Separator();
				}
			}
		}


		ImGui::End();
	}
}

void E_ScenePanel::CreateSceneMenu()
{
}

void E_ScenePanel::SceneInfo(R_Scene * sc)
{
}

void E_ScenePanel::ActiveSceneInfo(R_Scene* sc)
{
	if (!sc) return;

	ImGui::BeginChild("Active scene", ImVec2(200, 200));
	{
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), sc->GetNameCStr());

		ImGui::EndChild();
	}
}
