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


M_Editor::M_Editor() : Module("M_Editor", true)
{
	LOG_CREATION(moduleName.c_str());

	configuration = M_INIT | M_START | M_PRE_UPDATE | M_UPDATE | M_CLEAN_UP;
}


M_Editor::~M_Editor()
{
	LOG_DESTRUCTION(moduleName.c_str());
}

bool M_Editor::Init()
{
	LOG_INIT(moduleName.c_str());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplSDL2_InitForOpenGL(app->window->GetWindow(), app->render->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);

	ImGui::StyleColorsDark();

	return true;
}

bool M_Editor::Start()
{
	LOG_START(moduleName.c_str());

	geoPanel = new E_GeometryPanel();
	texPanel = new E_TexturePanel();
	scenePanel = new E_ScenePanel();

	panels.push_back(geoPanel);
	panels.push_back(texPanel);
	panels.push_back(scenePanel);

	return true;
}

UpdateReturn M_Editor::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(app->window->GetWindow());
	ImGui::NewFrame();

	return UpdateReturn::UPDT_CONTINUE;
}

UpdateReturn M_Editor::Update(float dt)
{
	ImGui::BeginMainMenuBar();
	{
		if(ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Demo", nullptr, showImGuiDemo)) showImGuiDemo = !showImGuiDemo;
			if (ImGui::MenuItem("Quit")) app->Close();

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Geometry", nullptr, geoPanel->Visible())) geoPanel->SwapShow();
			if (ImGui::MenuItem("Textures", nullptr, texPanel->Visible())) texPanel->SwapShow();
			if (ImGui::MenuItem("Scenes", nullptr, scenePanel->Visible())) scenePanel->SwapShow();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (showImGuiDemo) ImGui::ShowDemoWindow(&showImGuiDemo);

	for(auto it : panels)
	{
		if (it->Visible()) it->Display();
	}

	return UpdateReturn::UPDT_CONTINUE;
}

bool M_Editor::CleanUp()
{
	LOG_CLEANUP(moduleName.c_str());

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
