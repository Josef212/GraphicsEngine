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

	bool Loaded()const { return textureID != 0; }
	unsigned int TextureID()const { return textureID; }

	int Width()const { return width; }
	int Height()const { return height; }
	int NumChannels()const { return nrChannels; }
	const char* Path()const { return path.c_str(); }


private:
	unsigned int textureID = 0;
	int width = 0, height = 0, nrChannels = 0;

	std::string path = "";
};

#endif // !__R_TEXTURE_H__