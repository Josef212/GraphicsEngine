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
#include "E_InspectorPanel.h"
#include "E_MaterialsPanel.h"

#include "FileDialogue.h"

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
	m_inspector = new E_InspectorPanel();
	m_materialsPanel = new E_MaterialsPanel();

	m_panels.push_back(m_geoPanel);
	m_panels.push_back(m_texPanel);
	m_panels.push_back(m_scenePanel);
	m_panels.push_back(m_inspector);
	m_panels.push_back(m_materialsPanel);

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
			if (ImGui::MenuItem("File explorer", nullptr, m_fileDialogue.IsOpen())) m_fileDialogue.Open();
			if (ImGui::MenuItem("Demo", nullptr, m_showImGuiDemo)) m_showImGuiDemo = !m_showImGuiDemo;
			if (ImGui::MenuItem("Quit")) app->Close();

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Inspector", nullptr, m_inspector->Visible())) m_inspector->SwapShow();
			if (ImGui::MenuItem("Geometry", nullptr, m_geoPanel->Visible())) m_geoPanel->SwapShow();
			if (ImGui::MenuItem("Textures", nullptr, m_texPanel->Visible())) m_texPanel->SwapShow();
			if (ImGui::MenuItem("Scenes", nullptr, m_scenePanel->Visible())) m_scenePanel->SwapShow();
			if (ImGui::MenuItem("Materials", nullptr, m_materialsPanel->Visible())) m_materialsPanel->SwapShow();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (m_showImGuiDemo) ImGui::ShowDemoWindow(&m_showImGuiDemo);

	if (m_fileDialogue.IsOpen() && m_fileDialogue.Display("Data", nullptr))
	{
		if (m_fileDialogue.ValidFileSelected())
			LOG(LOG_INFO, m_fileDialogue.GetSelectedFile());
	}

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