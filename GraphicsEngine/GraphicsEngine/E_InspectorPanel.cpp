#include "E_InspectorPanel.h"

#include "App.h"
#include "EventManager.h"
#include "M_SceneManager.h"
#include "M_ResourceManager.h"
#include "R_Scene.h"
#include "R_Model.h"
#include "R_Geometry.h"
#include "R_Material.h"

#include "Light.h"
#include "Camera.h"


E_InspectorPanel::E_InspectorPanel() : Panel("E_InspectorPanel"), IEventListener()
{
	m_show = true;
}


E_InspectorPanel::~E_InspectorPanel()
{
}

void E_InspectorPanel::OnInit()
{
	app->eventManager->AddEventListener(this);
}

void E_InspectorPanel::OnDisable()
{
	app->eventManager->RemoveEventListener(this);
}

void E_InspectorPanel::Display()
{
	const int childWidth = 500;

	ImGui::Begin("E_Inspector", &m_show);

	R_Scene* scene = app->sceneManager->GetActiveScene();

	if(scene)
	{
		ImGui::Text("Active scene: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), scene->GetNameCStr());

		ImGui::ColorEdit3((scene->GetName() + " background").c_str(), &scene->m_backgroundColor.x);

		int modelsCount = scene->GetModelsCount();
		const int modelsHeight = 115 * modelsCount + 50;
		ImGui::BeginChild("Models", ImVec2(childWidth, modelsHeight), true);
		{
			ImGui::Text("Models: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", scene->GetModelsCount());

			ImGui::SameLine();

			if(ImGui::Button("Add model"))
			{
				R_Model* model = new R_Model("New model", app->resourceManager->defaultResources.quadGeo, app->resourceManager->defaultResources.simpleMat);
				scene->AddModel(model);
				modelsCount = scene->GetModelsCount();
			}

			ImGui::Separator(); ImGui::Separator();

			for(int i = 0; i < modelsCount; ++i)
			{
				R_Model* model = scene->GetModel(i);

				ModelDisplay(model);

				if(i < modelsCount - 1) ImGui::Separator();
			}

			ImGui::EndChild();
		}

		//ImGui::SameLine();

		int lightsCount = scene->GetLightsCount();
		const int lightsHeight = 75 * lightsCount + 40;
		ImGui::BeginChild("Lights", ImVec2(childWidth, lightsHeight), true);
		{
			ImGui::Text("Lights: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", scene->GetLightsCount());

			ImGui::SameLine();

			if(ImGui::Button("Add light"))
			{
				scene->AddLight(new Light());
				lightsCount = scene->GetLightsCount();
			}

			ImGui::Separator(); ImGui::Separator();

			for(int i = 0; i < lightsCount; ++i)
			{
				Light* light = scene->GetLight(i);

				LightDisplay(light, i);

				if (i < lightsCount - 1) ImGui::Separator();
			}

			ImGui::EndChild();
		}

		if(m_editingModel)
		{
			EditModel();
		}
	}


	ImGui::End();
}

void E_InspectorPanel::ModelDisplay(R_Model* model)
{
	ImGui::Text("Name: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), model->GetNameCStr());
	ImGui::SameLine(0, 50.f);
	if(ImGui::Button(("Edit: " + model->GetName()).c_str()))
	{
		m_editingModel = model;
	}

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

	static const char* types[] = { "POINT", "DIRECTIONAL", "SPOTLIGHT" };
	if (ImGui::Combo(("Light type: " + std::to_string(count)).c_str(), (int*)&type, types, 3)) light->m_type = type;
}

void E_InspectorPanel::EditModel()
{
	ImGui::Begin("Edit model", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	static char name[150];
	strcpy_s(name, 150, m_editingModel->GetNameCStr());

	ImGui::Text("Model name: "); ImGui::SameLine();
	if (ImGui::InputText("", name, 150)) m_editingModel->SetName(name);

	ImGui::Separator(); ImGui::Separator();

	if(ImGui::Button("Add mesh"))
	{
		m_editingModel->AddMesh(app->resourceManager->defaultResources.quadGeo, app->resourceManager->defaultResources.simpleMat);
	}

	int meshCount = m_editingModel->GetMeshCount();

	ImGui::BeginChild("Geometries", ImVec2(400, 400), true);
	{
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Geometry -------------------");

		for (int i = 0; i < meshCount; ++i)
		{
			auto geo = m_editingModel->GetGeometry(i);
			std::string text = "Select geometry" + std::to_string(i);

			if(ImGui::Selectable(("Geo " + std::to_string(i) + ": " + geo->GetName()).c_str()))
			{
				ImGui::OpenPopup(text.c_str());
			}

			if(ImGui::BeginPopup(text.c_str()))
			{
				auto res = ResourcePopup(RES_GEOMETRY);
				if(res) m_editingModel->SetGeometry(static_cast<R_Geometry*>(res), i);

				ImGui::EndPopup();
			}

			if (i < meshCount - 1) ImGui::Separator();
		}

		ImGui::EndChild();
	}

	ImGui::SameLine();

	ImGui::BeginChild("Materials", ImVec2(400, 400), true);
	{
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Materials -------------------");

		for(int i = 0; i < meshCount; ++i)
		{
			auto material = m_editingModel->GetMaterial(i);
			std::string text = "Select material" + std::to_string(i);

			if (ImGui::Selectable(("Mat " + std::to_string(i) + ": " + material->GetName()).c_str()))
			{
				ImGui::OpenPopup(text.c_str());
			}

			if (ImGui::BeginPopup(text.c_str()))
			{
				auto res = ResourcePopup(RES_MATERIAL);
				if (res) m_editingModel->SetMaterial(static_cast<R_Material*>(res), i);

				ImGui::EndPopup();
			}

			if (i < meshCount - 1) ImGui::Separator();
		}

		if(ImGui::Button("Change all materials"))
		{
			ImGui::OpenPopup("ChangeAllMaterials");
		}

		if(ImGui::BeginPopup("ChangeAllMaterials"))
		{
			auto res = ResourcePopup(RES_MATERIAL);
			if (res) m_editingModel->SetMaterial(static_cast<R_Material*>(res));

			ImGui::EndPopup();
		}

		ImGui::EndChild();
	}

	if(ImGui::Button("Close"))
	{
		m_editingModel = nullptr;
	}

	ImGui::End();
}

EventType E_InspectorPanel::GetSupportedEvents()
{
	return EventType::EVENT_RESOURCE_REMOVED;
}

void E_InspectorPanel::OnEventRecieved(Event e)
{
	if(e.type == EventType::EVENT_RESOURCE_REMOVED)
	{
		m_editingModel = static_cast<R_Model*>(app->resourceManager->ValidateResource(m_editingModel));
	}
}
