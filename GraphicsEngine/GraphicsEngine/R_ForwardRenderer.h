#ifndef __R_FORWARDRENDERER_H__
#define __R_FORWARDRENDERER_H__

#include "R_Renderer.h"

class FrameBuffer;

class R_ForwardRenderer : public R_Renderer
{
public:
	R_ForwardRenderer(const char* name);
	virtual ~R_ForwardRenderer();

	void RenderScene(R_Scene* scene) override;

	FrameBuffer* frameBuffer;
};

#endif // !__R_FORWARDRENDERER_H__