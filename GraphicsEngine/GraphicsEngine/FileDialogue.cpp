#include "FileDialogue.h"

#include "App.h"
#include "M_FileSystem.h"

#include "ThirdParty/ImGui/imgui.h"
#include <algorithm>


FileDialogue::FileDialogue()
{
	m_selectedFile[0] = '\0';
	m_shouldClose = false;
}


FileDialogue::~FileDialogue()
{
}

bool FileDialogue::Display(const char* dir, const char* filterExt)
{
	bool ret = false;

	m_shouldClose = false;

	ImGui::OpenPopup("File dialogue");

	if(ImGui::BeginPopupModal("File dialogue", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("File browser", ImVec2(0, 300), true);

		Directories(dir, filterExt);

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::PushItemWidth(250.0f);
		if (ImGui::InputText("##file_selector", m_selectedFile, MAX_FILE_SIZE, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			;
		}

		ImGui::PopItemWidth();

		ImGui::SameLine();
		if (ImGui::Button("Ok", ImVec2(50, 20)))
		{
			ret = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(50, 20)))
		{
			m_selectedFile['\0'];
			ret = true;
		}

		ImGui::EndPopup();
	}

	if (ret || m_shouldClose) m_state = FileDialogueState::FDS_CLOSE;

	return ret || m_shouldClose;
}

void FileDialogue::Directories(const char* dir, const char* filterExt)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;

	std::string directory(dir ? dir : "");
	directory += "/";

	app->fs->GetFilesAndDirs(directory.c_str(), files, dirs);

	for (auto it : dirs)
	{
		if (ImGui::TreeNodeEx((directory + it).c_str(), 0, it.c_str()))
		{
			Directories((directory + it).c_str(), filterExt);
			ImGui::TreePop();
		}
	}

	std::sort(files.begin(), files.end());

	for (auto it : files)
	{
		const std::string str = it;
		bool ok = true;

		if (filterExt && str.substr(str.find_last_of(".") + 1) != filterExt)
			ok = false;

		if (ok && ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked())
			{
				sprintf_s(m_selectedFile, MAX_FILE_SIZE, "%s%s", directory.c_str(), str.c_str());

				if (ImGui::IsMouseDoubleClicked(0))
					m_shouldClose = true;
			}

			ImGui::TreePop();
		}
	}
}
