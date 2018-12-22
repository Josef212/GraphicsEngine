#pragma once

#include "R_Material.h"

class R_Texture;

class R_Mat_SimpleTexture : public R_Material
{
public:
	R_Mat_SimpleTexture(const char* name);
	virtual ~R_Mat_SimpleTexture();

	void OnFree() override;

	void SendInfo(R_Scene* scene)const override;

public:
	R_Texture* texture = nullptr;
};

