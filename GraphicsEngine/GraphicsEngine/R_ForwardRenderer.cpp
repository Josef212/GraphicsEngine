#include "R_ForwardRenderer.h"

#include "R_Scene.h"
#include "R_Geometry.h"
#include "R_Shader.h"
#include "R_Material.h"
#include "R_Model.h"
#include "R_Texture.h"

#include "Camera.h"
#include "PerspectiveCamera.h"

#include "OpenGL.h"

R_ForwardRenderer::R_ForwardRenderer(const char * name) : R_Renderer(name)
{
}

R_ForwardRenderer::~R_ForwardRenderer()
{
}

void R_ForwardRenderer::RenderScene(R_Scene * scene)
{
	glClearColor(scene->m_backgroundColor.r, scene->m_backgroundColor.g, scene->m_backgroundColor.b, scene->m_backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto models = scene->GetModels();
	for (auto it : *models)
	{
		it->Render(scene);
	}
}
