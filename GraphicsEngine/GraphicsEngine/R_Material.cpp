#include "R_Material.h"

#include "R_Shader.h"


R_Material::R_Material(const char* name) : Resource(name, RES_MATERIAL)
{

}

R_Material::R_Material(const char* name, R_Shader* sh) : Resource(name, RES_MATERIAL), m_shader(sh)
{
	
}

R_Material::~R_Material()
{
}

void R_Material::Load()
{
	OnLoad();
}

void R_Material::Free()
{
	OnFree();
}

void R_Material::InitRender() const
{
	if (m_shader) m_shader->Use();
}

void R_Material::EndRender() const
{
	glUseProgram(0);
}
