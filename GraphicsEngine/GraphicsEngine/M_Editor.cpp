#include "M_Editor.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_sdl.h"

#include "App.h"
#include "M_Window.h"
#include "M_Render3D.h"
#include "ThirdParty/ImGui/imgui_impl_opengl3.h"

#include "E_GeometryPanel.h"
#include "E_TexturePanel.h"
#include "E_ScenePanel.h"

#include "M_FileSystem.h"
#include <algorithm>


M_Editor::M_Editor() : Module("M_Editor", true)
{
	LOG_CREATION(m_moduleName.c_str());

	m_configuration = M_INIT | M_START | M_PRE_UPDATE | M_UPDATE | M_CLEAN_UP;
}


M_Editor::~M_Editor()
{
	LOG_DESTRUCTION(m_moduleName.c_str());
}

bool M_Editor::Init()
{
	LOG_INIT(m_moduleName.c_str());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplSDL2_InitForOpenGL(app->window->GetWindow(), app->render->m_context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);

	ImGui::StyleColorsDark();

	return true;
}

bool M_Editor::Start()
{
	LOG_START(m_moduleName.c_str());

	m_geoPanel = new E_GeometryPanel();
	m_texPanel = new E_TexturePanel();
	m_scenePanel = new E_ScenePanel();

	m_panels.push_back(m_geoPanel);
	m_panels.push_back(m_texPanel);
	m_panels.push_back(m_scenePanel);

	return true;
}

UpdateReturn M_Editor::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(app->window->GetWindow());
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	m_isUsingMouse = io.WantCaptureMouse;
	m_isUsingKeyboard = io.WantCaptureKeyboard;

	return UpdateReturn::UPDT_CONTINUE;
}

UpdateReturn M_Editor::Update(float dt)
{
	ImGui::BeginMainMenuBar();
	{
		if(ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("File explorer", nullptr, m_fileExplorerOpen)) m_fileExplorerOpen = true;
			if (ImGui::MenuItem("Demo", nullptr, m_showImGuiDemo)) m_showImGuiDemo = !m_showImGuiDemo;
			if (ImGui::MenuItem("Quit")) app->Close();

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Geometry", nullptr, m_geoPanel->Visible())) m_geoPanel->SwapShow();
			if (ImGui::MenuItem("Textures", nullptr, m_texPanel->Visible())) m_texPanel->SwapShow();
			if (ImGui::MenuItem("Scenes", nullptr, m_scenePanel->Visible())) m_scenePanel->SwapShow();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (m_showImGuiDemo) ImGui::ShowDemoWindow(&m_showImGuiDemo);

	if (m_fileExplorerOpen) FileExplorer("Data", nullptr);

	for(auto it : m_panels)
	{
		if (it->Visible()) it->Display();
	}

	return UpdateReturn::UPDT_CONTINUE;
}

bool M_Editor::CleanUp()
{
	LOG_CLEANUP(m_moduleName.c_str());

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void M_Editor::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void M_Editor::PassInput(SDL_Event* ev)
{
	ImGui_ImplSDL2_ProcessEvent(ev);
}

void M_Editor::FileExplorer(const char* dir, const char* filterExt)
{
	ImGui::OpenPopup("File dialogue");

	if(ImGui::BeginPopupModal("File dialogue", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("File browser", ImVec2(0, 300), true);

		Directories(dir, filterExt);

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::PushItemWidth(250.0f);
		if (ImGui::InputText("##file_selector", m_selectedFile, FILE_MAX_SIZE, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			;

		ImGui::PopItemWidth();

		ImGui::SameLine();
		if (ImGui::Button("Ok", ImVec2(50, 20)))
		{
			m_fileExplorerOpen = false;
		}

		ImGui::SameLine();
		if(ImGui::Button("Cancel", ImVec2(50, 20)))
		{
			m_selectedFile['\0'];
			m_fileExplorerOpen = false;
		}

		ImGui::EndPopup();
	}
}

void M_Editor::Directories(const char* dir, const char* filterExt)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;

	std::string directory(dir ? dir : "");
	directory += "/";

	app->fs->GetFilesAndDirs(directory.c_str(), files, dirs);

	for(auto it : dirs)
	{
		if(ImGui::TreeNodeEx((directory + it).c_str(), 0, it.c_str()))
		{
			Directories((directory + it).c_str(), filterExt);
			ImGui::TreePop();
		}
	}

	std::sort(files.begin(), files.end());

	for(auto it : files)
	{
		const std::string str = it;
		bool ok = true;

		if (filterExt && str.substr(str.find_last_of(".") + 1) != filterExt)
			ok = false;

		if(ok && ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if(ImGui::IsItemClicked())
			{
				sprintf_s(m_selectedFile, FILE_MAX_SIZE, "%s%S", directory.c_str(), str.c_str());

				if (ImGui::IsMouseDoubleClicked(0))
					;// Close file dialogue
			}

			ImGui::TreePop();
		}
	}
}
