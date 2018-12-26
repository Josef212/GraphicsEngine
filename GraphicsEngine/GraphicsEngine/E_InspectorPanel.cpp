#include "E_InspectorPanel.h"

#include "App.h"
#include "M_SceneManager.h"
#include "M_ResourceManager.h"
#include "R_Scene.h"
#include "R_Model.h"

#include "Light.h"
#include "Camera.h"


E_InspectorPanel::E_InspectorPanel() : Panel("E_InspectorPanel")
{
}


E_InspectorPanel::~E_InspectorPanel()
{
}

void E_InspectorPanel::OnInit()
{
}

void E_InspectorPanel::Display()
{
	ImGui::Begin("E_Inspector");

	R_Scene* scene = app->sceneManager->GetActiveScene();

	if(scene)
	{
		ImGui::Text("Active scene: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), scene->GetNameCStr());

		ImGui::Text("Num. models: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", scene->GetModelsCount());

		ImGui::SameLine(0.f, 75.f);

		ImGui::Text("Num. lights: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", scene->GetLightsCount());

		int modelsHeight = 110 * scene->GetModelsCount() + 40;
		ImGui::BeginChild("Models", ImVec2(500, modelsHeight), true);
		{
			ImGui::Text("Models");
			ImGui::Separator(); ImGui::Separator();

			for(int i = 0; i < scene->GetModelsCount(); ++i)
			{
				R_Model* model = scene->GetModel(i);

				ModelDisplay(model);

				if(i < scene->GetModelsCount() - 1) ImGui::Separator();
			}

			ImGui::EndChild();
		}

		//ImGui::SameLine();

		int lightsHeight = 70 * scene->GetLightsCount() + 40;
		ImGui::BeginChild("Lights", ImVec2(500, lightsHeight), true);
		{
			ImGui::Text("Lights");
			ImGui::Separator(); ImGui::Separator();

			for(int i = 0; i < scene->GetLightsCount(); ++i)
			{
				Light* light = scene->GetLight(i);

				LightDisplay(light, i);

				if (i < scene->GetModelsCount() - 1) ImGui::Separator();
			}

			ImGui::EndChild();
		}
	}


	ImGui::End();
}

void E_InspectorPanel::ModelDisplay(R_Model* model)
{
	ImGui::Text("Name: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), model->GetNameCStr());

	glm::vec3 t = model->GetTranslation();
	glm::vec3 e = model->GetEuler();
	glm::vec3 s = model->GetScale();

	if (ImGui::DragFloat3((model->GetName() + ": Translation").c_str(), &t.x, 0.1f)) model->SetTranslation(t);
	if (ImGui::DragFloat3((model->GetName() + ": Rotation").c_str(), &e.x, 0.1f, -360.f, 360.f)) model->SetEuler(e);
	if (ImGui::DragFloat3((model->GetName() + ": Scale").c_str(), &s.x, 0.01f)) model->SetScale(s);

	if(ImGui::Button(("Reset " + model->GetName() + " transform").c_str()))
	{
		model->SetScale(glm::vec3(1.f));
		model->SetEuler(glm::vec3(0.f));
		model->SetTranslation(glm::vec3(0.f));
	}
}

void E_InspectorPanel::LightDisplay(Light* light, int count)
{
	glm::vec3 position = light->m_position;
	glm::vec3 color = light->m_color;
	LightType type = light->m_type;

	if (ImGui::DragFloat3(("Light pos: " + std::to_string(count)).c_str(), &position.x, 0.1f)) 
		light->m_position = position;
	if (ImGui::ColorEdit3(("Light color: " + std::to_string(count)).c_str(), &color.x)) light->m_color = color;

	const char* types[] = { "POINT", "DIRECTIONAL", "SPOTLIGHT" };
	if (ImGui::Combo(("Light type: " + std::to_string(count)).c_str(), (int*)&type, types, 3)) light->m_type = type;
}
