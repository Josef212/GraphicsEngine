#include "E_MaterialsPanel.h"

#include "App.h"
#include "EventManager.h"
#include "M_ResourceManager.h"

#include "R_Shader.h"
#include "R_Material.h"
#include "R_Mat_SimpleTexture.h"
#include "R_ComplexMaterial.h"

#include "R_Texture.h"



E_MaterialsPanel::E_MaterialsPanel() : Panel("E_MaterialsPanel"), IEventListener()
{
}

E_MaterialsPanel::~E_MaterialsPanel()
{
}

EventType E_MaterialsPanel::GetSupportedEvents()
{
	return EventType::EVENT_RESOURCE_REMOVED;
}

void E_MaterialsPanel::OnEventRecieved(Event e)
{
	if (e.type == EventType::EVENT_RESOURCE_REMOVED)
		m_editingMaterial = dynamic_cast<R_Material*>(app->resourceManager->ValidateResource(m_editingMaterial));
}

void E_MaterialsPanel::OnInit()
{
	app->eventManager->AddEventListener(this);
}

void E_MaterialsPanel::OnDisable()
{
	app->eventManager->RemoveEventListener(this);
	m_editingMaterial = nullptr;
}

void E_MaterialsPanel::Display()
{
	ImGui::Begin(m_name.c_str(), &m_show);
	{
		std::vector<Resource*> materials;
		app->resourceManager->GatherResourcesOfType(ResourceType::RES_MATERIAL, materials);

		for(auto it : materials)
		{
			if (ImGui::Selectable(it->GetNameCStr()))
				m_editingMaterial = dynamic_cast<R_Material*>(it);
		}

		ImGui::Separator();

		if (m_editingMaterial)
			EditMaterial();

		// TODO: Add a preview of the material
		ImGui::Separator();

		if (ImGui::Button("Create material")) ImGui::OpenPopup("CreateMaterial");
		ImGui::SameLine();
		if (ImGui::Button("Close")) Disable();

		CreateMaterial();

		ImGui::End();
	}
}

void E_MaterialsPanel::EditMaterial()
{
	ImGui::Separator();

	char name[150];
	strcpy_s(name, 150, m_editingMaterial->GetNameCStr());

	ImGui::Text("Material name: "); ImGui::SameLine();
	ImGui::InputText("", name, 150);

	Panel::ShaderInfo(m_editingMaterial->GetShader());
	auto sh = Panel::ChangeShaderMenu();
	if (sh) m_editingMaterial->SetShader(sh);

	ImGui::Separator();

	m_editingMaterial->OnEditUI();

	ImGui::Separator();
}

void E_MaterialsPanel::CreateMaterial()
{
	if (ImGui::BeginPopupModal("CreateMaterial", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		// ------------------------------------------
		static const char* types[] = { "INT", "INT_PTR", "FLOAT", "FLOAT_PTR", "BOOL", "BOOL_PTR", "TEXTURE",
		"VEC2", "VEC3", "VEC4", "MAT3", "MAT4" };
		static char name[150];
		static R_Shader* shader = nullptr;
		static std::vector<MatProperty*> properties;
		static MatPropertyValueType propType = MatPropertyValueType::MAT_INT;

		//-----

		ImGui::InputText("Material name", name , 150);

		Panel::ShaderInfo(shader);
		auto sh = Panel::ChangeShaderMenu();
		if (sh) shader = sh;

		ImGui::Combo("Property type", (int*)&propType, types, 12); ImGui::SameLine();
		if(ImGui::Button("Add property"))
		{
			properties.push_back(new MatProperty("New property", propType));
			propType = MatPropertyValueType::MAT_INT;
		}

		ImGui::Separator();
		
		// ===

		static std::vector<MatProperty*> removeVector;

		// Current properties 
		int i = 0;
		for(auto p : properties)
		{
			std::string label = std::to_string(i);

			static char propName[150];
			strcpy_s(propName, 150, p->m_propertyName.c_str());
			if (ImGui::InputText((label + " name").c_str(), propName, 150)) p->m_propertyName = propName;

			if(p->m_propertyType != MAT_INT_PTR && p->m_propertyType != MAT_FLOAT_PTR && p->m_propertyType != MAT_BOOL_PTR)
				ImGui::SameLine();
			
			switch (p->m_propertyType)
			{
			case MatPropertyValueType::MAT_INT:
				ImGui::DragInt(label.c_str(), &p->m_propertyValue._int);
				break;

			case MatPropertyValueType::MAT_INT_PTR:
				//ImGui::DragInt(label.c_str(), p->m_propertyValue._intPtr);
				break;

			case MatPropertyValueType::MAT_FLOAT:
				ImGui::DragFloat(label.c_str(), &p->m_propertyValue._float);
				break;

			case MatPropertyValueType::MAT_FLOAT_PTR:
				//ImGui::DragFloat(label.c_str(), p->m_propertyValue._floatPtr);
				break;

			case MatPropertyValueType::MAT_BOOL:
				ImGui::Checkbox(label.c_str(), &p->m_propertyValue._bool);
				break;

			case MatPropertyValueType::MAT_BOOL_PTR:
				//ImGui::Checkbox(label.c_str(), p->m_propertyValue._boolPtr);
				break;

			case MatPropertyValueType::MAT_VEC2:
				ImGui::DragFloat2(label.c_str(), &p->m_propertyValue._vec2.x);
				break;

			case MatPropertyValueType::MAT_VEC3:
				ImGui::DragFloat3(label.c_str(), &p->m_propertyValue._vec3.x);
				break;

			case MatPropertyValueType::MAT_VEC4:
				ImGui::DragFloat4(label.c_str(), &p->m_propertyValue._vec4.x);
				break;

			case MatPropertyValueType::MAT_MAT3:
				ImGui::Text(label.c_str()); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "MAT3");
				break;

			case MatPropertyValueType::MAT_MAT4:
				ImGui::Text(label.c_str()); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "MAT4");
				break;

			case MatPropertyValueType::MAT_TEXTURE:
				R_Texture* propTexture = p->m_propertyValue._texture;
				std::string text = "Select texture: " + p->m_propertyName;

				if (ImGui::ImageButton((ImTextureID)(propTexture ? propTexture->TextureID() : 0), ImVec2(32, 32)))
				{
					ImGui::OpenPopup(text.c_str());
				}

				auto tex = Panel::ResourcePopup<R_Texture*>(RES_TEXTURE, text.c_str());
				if (tex) p->m_propertyValue._texture = tex;

				break;
			}

			ImGui::SameLine();
			if (ImGui::Button(("Remove " + label).c_str())) removeVector.push_back(p);

			++i;
		}

		// Remove deleted properties
		for(auto p : removeVector)
		{
			auto it = std::find(properties.begin(), properties.end(), p);
			if(it != properties.end())
			{
				RELEASE(p);
				properties.erase(it);
			}
		}

		removeVector.clear();
		
		// ------------------------------------------

		ImGui::Separator();

		if(ImGui::Button("Create"))
		{
			R_ComplexMaterial* mat = new R_ComplexMaterial(name, shader);
			for(auto it : properties)
			{
				mat->AddProperty(it);
			}

			strcpy_s(name, 150, "New material");
			propType = MatPropertyValueType::MAT_INT;
			properties.clear();
			shader = nullptr;

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			strcpy_s(name, 150, "New material");
			propType = MatPropertyValueType::MAT_INT;

			for (auto it : properties) RELEASE(it);
			properties.clear();
			shader = nullptr;

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

// =================================

void R_Mat_SimpleTexture::OnEditUI()
{
	if(ImGui::ImageButton((ImTextureID)(texture ? texture->TextureID() : 0), ImVec2(32, 32)))
	{
		ImGui::OpenPopup("SelectTexture");
	}
	ImGui::SameLine();
	ImGui::Text("Texture");

	auto tex = Panel::ResourcePopup<R_Texture*>(RES_TEXTURE, "SelectTexture");
	if (tex) texture = tex;
}

void R_ComplexMaterial::OnEditUI()
{
	ImGui::Text("Properties -----------------");

	int i = 0;
	for(auto prop : m_properties)
	{
		if (!prop) continue;

		std::string label = std::to_string(i) + prop->m_propertyName;

		switch (prop->m_propertyType)
		{
		case MatPropertyValueType::MAT_INT:
			ImGui::DragInt(label.c_str(), &prop->m_propertyValue._int);
			break;

		case MatPropertyValueType::MAT_INT_PTR:
			ImGui::DragInt(label.c_str(), prop->m_propertyValue._intPtr);
			break;

		case MatPropertyValueType::MAT_FLOAT:
			ImGui::DragFloat(label.c_str(), &prop->m_propertyValue._float);
			break;

		case MatPropertyValueType::MAT_FLOAT_PTR:
			ImGui::DragFloat(label.c_str(), prop->m_propertyValue._floatPtr);
			break;

		case MatPropertyValueType::MAT_BOOL:
			ImGui::Checkbox(label.c_str(), &prop->m_propertyValue._bool);
			break;

		case MatPropertyValueType::MAT_BOOL_PTR:
			ImGui::Checkbox(label.c_str(), prop->m_propertyValue._boolPtr);
			break;

		case MatPropertyValueType::MAT_VEC2:
			ImGui::DragFloat2(label.c_str(), &prop->m_propertyValue._vec2.x);
			break;

		case MatPropertyValueType::MAT_VEC3:
			ImGui::DragFloat3(label.c_str(), &prop->m_propertyValue._vec3.x);
			break;

		case MatPropertyValueType::MAT_VEC4:
			ImGui::DragFloat4(label.c_str(), &prop->m_propertyValue._vec4.x);
			break;

		case MatPropertyValueType::MAT_MAT3:
			ImGui::Text(label.c_str()); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "MAT3");
			break;

		case MatPropertyValueType::MAT_MAT4:
			ImGui::Text(label.c_str()); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "MAT4");
			break;

		case MatPropertyValueType::MAT_TEXTURE:
			R_Texture* propTexture = prop->m_propertyValue._texture;
			std::string text = "Select texture: " + label;

			if(ImGui::ImageButton((ImTextureID)(propTexture ? propTexture->TextureID() : 0), ImVec2(32, 32)))
			{
				ImGui::OpenPopup(text.c_str());
			}
			ImGui::SameLine(); ImGui::Text(prop->m_propertyName.c_str());

			auto tex = Panel::ResourcePopup<R_Texture*>(RES_TEXTURE, text.c_str());
			if (tex) prop->m_propertyValue._texture = tex;

			break;
		}

		++i;
	}
}
