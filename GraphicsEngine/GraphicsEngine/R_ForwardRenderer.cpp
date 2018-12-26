#include "R_ForwardRenderer.h"

//TMP
#include "App.h"
#include "M_ResourceManager.h"
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

}

R_ForwardRenderer::~R_ForwardRenderer()
{
}

void R_ForwardRenderer::RenderScene(R_Scene * scene)
{
	assert(scene != nullptr);

	ClearPass(scene);

	if (!scene->GetActiveCamera()) return;

	RenderModels(scene);

	DebugRenderLights(scene);
}
