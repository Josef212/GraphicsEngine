#include "E_GeometryPanel.h"

#include "Resource.h"
#include "R_Geometry.h"

#include "App.h"
#include "M_ResourceManager.h"

E_GeometryPanel::E_GeometryPanel() : Panel("E_GeometryPanel")
{
}


E_GeometryPanel::~E_GeometryPanel()
{
}

void E_GeometryPanel::OnInit()
{
}

void E_GeometryPanel::Display()
{
	ImGui::Begin(m_name.c_str(), &m_show);
	{
		std::vector<Resource*> geos;
		app->resourceManager->GatherResourcesOfType(RES_GEOMETRY, geos);

		GeometryRender();
		
		for(int i = 0; i < geos.size(); ++i)
		{
			ImGuiTreeNodeFlags flags = 0;

			R_Geometry* geo = static_cast<R_Geometry*>(geos[i]);
			if(geo)
			{
				if(ImGui::TreeNodeEx(geo->GetNameCStr(), flags))
				{
					if (ImGui::IsItemClicked()) m_selectedGeo = static_cast<R_Geometry*>(geos[i]);

					GeometryInfo(geo);

					ImGui::TreePop();
				}

				if (i < geos.size() - 1)
				{
					ImGui::Separator(); 
					ImGui::Separator();
				}
			}
		}

		ImGui::End();
	}
}

void E_GeometryPanel::GeometryInfo(R_Geometry * geo)
{
	ImGui::Text("Num vertices: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", geo->CountVertices());

	ImGui::Text("Num indices: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", geo->CountIndices());

	ImGui::Separator();

	// ----

	ImGui::Text("EBO: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", geo->EBO());

	ImGui::Text("Id indices: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", geo->IdIndices());

	ImGui::Text("Id vertices: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", geo->IdVertices());

	ImGui::Text("Id normals: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", geo->IdNormals());

	ImGui::Text("Id uvs: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", geo->IdUvs());

	ImGui::Text("Id colors: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", geo->IdColors());
}

void E_GeometryPanel::GeometryRender()
{
	if(!m_selectedGeo) return;

	ImGui::BeginChild("Geo", ImVec2(200, 200), true);
	{
		ImGui::Text(m_selectedGeo->GetNameCStr());
		//TODO
		// Bind frame buffer

		// Render geometry

		// Unbind frame buffer

		// Display image


		ImGui::EndChild();
	}
}
