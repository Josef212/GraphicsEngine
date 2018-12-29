#include "R_ForwardRenderer.h"

//TMP
#include "App.h"
#include "M_ResourceManager.h"
#include "FrameBuffer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "M_Window.h"
// ===========

#include "R_Scene.h"
#include "R_Geometry.h"
#include "R_Shader.h"
#include "R_Material.h"
#include "R_Model.h"
#include "R_Texture.h"

#include "Camera.h"

#include "OpenGL.h"

R_ForwardRenderer::R_ForwardRenderer(const char * name) : R_Renderer(name)
{
	frameBuffer = new FrameBuffer(app->window->GetWinWidth(), app->window->GetWinHeight());
	//frameBuffer->Create();
	frameBuffer->Bind();

	TextureConfig t0, t1, t2, t3;
	CreateDefaultColorTexture(t0);
	CreateDefaultDepthTexture(t1);
	CreateDefaultStencilTexture(t2);
	CreateDefaultDepthStencilTexture(t3);

	frameBuffer->CreateTexture(t0);
	//frameBuffer->CreateTexture(t1);
	//frameBuffer->CreateTexture(t2);
	frameBuffer->CreateTexture(t3);

	//frameBuffer->CreateRenderBufferObject(GL_DEPTH24_STENCIL8, Attachment::A_DEPTH_STENCIL);
	
	frameBuffer->UnBind();
}

R_ForwardRenderer::~R_ForwardRenderer()
{
}

void R_ForwardRenderer::RenderScene(R_Scene * scene)
{
	assert(scene != nullptr);

	//frameBuffer->Bind();

	ClearPass(scene);

	if (!scene->GetActiveCamera()) return;

	RenderModels(scene);
	DebugRenderLights(scene);

	//frameBuffer->UnBind();

	ImGui::Begin("Framebuffers", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	for(int i = 0; i < MAX_FB_TEXTURES; ++i)
	{
		ImGui::Image((ImTextureID)frameBuffer->GetTexture(i), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
		
		if(((i + 1) % 3) != 0) ImGui::SameLine();
	}

	ImGui::End();
}
