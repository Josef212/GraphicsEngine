#ifndef __R_RENDERER_H__
#define __R_RENDERER_H__

#include "Resource.h"

class R_Scene;
class R_Model;
class R_Geometry;
class R_Material;

#define NO_RENDER_CONFIG 0
#define PASS_MODEL 1
#define PASS_VIEW 2
#define PASS_PROJ 4
#define PASS_PROJ_VIEW_MODEL  PASS_MODEL | PASS_VIEW | PASS_PROJ

class R_Renderer : public Resource
{
public:
	R_Renderer(const char* name);
	virtual ~R_Renderer();

	virtual void RenderScene(R_Scene* scene) = 0;

protected:
	void ClearPass(R_Scene* scene);
	void ClearGeometryBuffer();

	void RenderModels(R_Scene* scene, int renderConfig = PASS_PROJ_VIEW_MODEL);
	void RenderModel(R_Model* model, R_Scene* scene, int renderConfig = PASS_PROJ_VIEW_MODEL);
	void RenderMesh(R_Geometry* geometry, R_Material* material, R_Model* model, R_Scene* scene, int renderConfig = PASS_PROJ_VIEW_MODEL);
	void RenderGeometry(R_Geometry* geometry);

	void DebugRenderLights(R_Scene* scene);

};

#endif // !__R_RENDERER_H__