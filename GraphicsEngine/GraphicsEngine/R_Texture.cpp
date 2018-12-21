#include "R_Texture.h"

#include <SOIL.h>
#include <GL/glew.h>

#include <iostream>
#include <filesystem>


R_Texture::R_Texture(const char* name) : Resource(name, RES_TEXTURE)
{
}


R_Texture::~R_Texture()
{
}

void R_Texture::Load()
{
	if(!m_path.empty())
	{
		Free();
		LoadTexture(Path());
	}
}

void R_Texture::Free()
{
	if (Loaded()) glDeleteTextures(1, &m_textureID);
}

// TODO: Change loading to FS
void R_Texture::LoadTexture(const char * path)
{
	this->m_path = path;

	if (Loaded()) Free();

	std::cout << "Loading texture: " << this->m_path << std::endl;

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	unsigned char *data = SOIL_load_image(this->m_path.c_str(), &m_width, &m_height, &m_nrChannels, SOIL_LOAD_RGBA);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		m_textureID = 0;
	}

	SOIL_free_image_data(data);

	glBindTexture(GL_TEXTURE_2D, 0);
}
