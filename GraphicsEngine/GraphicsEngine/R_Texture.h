#ifndef __R_TEXTURE_H__
#define __R_TEXTURE_H__

#include "Resource.h"

class R_Texture : public Resource
{
public:
	R_Texture(const char* name);
	virtual ~R_Texture();

	void Load() override;
	void Free() override;

	void LoadTexture(const char* path);

	bool Loaded()const { return m_textureID != 0; }
	unsigned int TextureID()const { return m_textureID; }

	int Width()const { return m_width; }
	int Height()const { return m_height; }
	int NumChannels()const { return m_nrChannels; }
	const char* Path()const { return m_path.c_str(); }


private:
	unsigned int m_textureID = 0;
	int m_width = 0, m_height = 0, m_nrChannels = 0;

	std::string m_path = "";
};

#endif // !__R_TEXTURE_H__