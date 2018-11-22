#pragma once

#include "Module.h"

class TestScene : public Module
{
public:
	TestScene();
	virtual ~TestScene();

	bool Init() override;
	UpdateReturn PreUpdate(float dt) override;
	UpdateReturn Update(float dt) override;
	bool CleanUp() override;


};

