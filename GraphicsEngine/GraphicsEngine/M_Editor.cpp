#include "M_Editor.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_sdl.h"

#include "App.h"
#include "M_Window.h"
#include "M_Render3D.h"
#include "ThirdParty/ImGui/imgui_impl_opengl3.h"


M_Editor::M_Editor() : Module("M_Editor", true)
{

	configuration = M_INIT | M_START | M_PRE_UPDATE | M_UPDATE | M_CLEAN_UP;
}


M_Editor::~M_Editor()
{
}

bool M_Editor::Init()
{
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
		

		ImGui::EndMainMenuBar();
	}

	return UpdateReturn::UPDT_CONTINUE;
}

bool M_Editor::CleanUp()
{
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
