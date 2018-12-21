#ifndef __R_RENDERER_H__
#define __R_RENDERER_H__

#include "Resource.h"

class R_Scene;

class R_Renderer : public Resource
{
public:
	R_Renderer(const char* name);
	virtual ~R_Renderer();

	virtual void RenderScene(R_Scene* scene) = 0;
};

#endif // !__R_RENDERER_H__