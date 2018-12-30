#include "Panel.h"

#include "R_Shader.h"

void Panel::ShaderInfo(R_Shader * shader)
{
	ImGui::Text("Shader name: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), shader ? shader->GetNameCStr() : "NONE");

	ImGui::Text("\tShader status: "); ImGui::SameLine();
	ImVec4 statusCol(1, 0, 0, 1);
	if (shader)
	{
		switch (shader->GetStatus())
		{
		case Shader_Status::SH_COMPILED:
			statusCol = { 0, 1, 0, 1 };
			break;

		case Shader_Status::SH_PENDING:
			statusCol = { 1, 1, 0, 1 };
			break;
		}
	}

	ImGui::TextColored(statusCol, shader ? shader->GetStatusStr() : "NO SHADER");
}

R_Shader* Panel::ChangeShaderMenu(bool sameLine)
{
	if (sameLine) ImGui::SameLine(0, 75.f);
	if (ImGui::Button("Change shader")) ImGui::OpenPopup("Change shader");
	return ResourcePopup<R_Shader*>(RES_SHADER, "Change shader");
}
