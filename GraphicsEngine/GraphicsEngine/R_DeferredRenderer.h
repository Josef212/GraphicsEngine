#ifndef __R_DEFERREDRENDERER_H__
#define __R_DEFERREDRENDERER_H__

#include "R_Renderer.h"

class FrameBuffer;
class R_Shader;
class R_Texture;

class R_DeferredRenderer : public R_Renderer
{
public:
	R_DeferredRenderer(const char* name);
	virtual ~R_DeferredRenderer();

	void RenderScene(R_Scene* scene) override;

	FrameBuffer* m_gBuffer = nullptr;

	R_Shader* m_deferredShader = nullptr;
	R_Shader* m_frameShader = nullptr;
	R_Texture* m_diffuse = nullptr;
	R_Texture* m_specular = nullptr;
};

#endif // !__R_DEFERREDRENDERER_H__