#include "R_Mat_SimpleTexture.h"

#include "R_Texture.h"
#include "R_Shader.h"

R_Mat_SimpleTexture::R_Mat_SimpleTexture(const char* name) : R_Material(name)
{
}


R_Mat_SimpleTexture::~R_Mat_SimpleTexture()
{
	
}

void R_Mat_SimpleTexture::OnFree()
{
}

void R_Mat_SimpleTexture::SendInfo(R_Scene * scene) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->TextureID());
	m_shader->SetInt("textureColor", 0);
}
