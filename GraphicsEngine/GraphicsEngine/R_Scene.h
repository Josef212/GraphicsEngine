#ifndef __R_SCENE_H__
#define __R_SCENE_H__

#include "Resource.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Camera;
class R_Model;
class R_Shader;
class R_Material;
enum CameraMovement;

#define MAX_CAMERAS 9

class R_Scene : public Resource
{
public:
	R_Scene(const char* name);
	virtual ~R_Scene();

	void Load() override;
	void Free() override;

	virtual void OnLoad() {}
	virtual void OnFree() {}

	virtual void OnActiveCameraChanged() {}

	void AddModel(R_Model* model);
	void RemoveModel(R_Model* model);
	void RemoveModel(const char* name);

	void AddCamera(Camera* cam, int index, bool setAsActive = false);
	void RemoveCamera(int index);

	void SetActiveCamera(int index);
	Camera* GetActiveCamera() const;

	void OnResize(int w, int h);

	int Width()const { return viewportWidth; }
	int Height()const { return viewportHeight; }

	R_Material* GetDefaultMaterial()const;

	void RenderScene(); // TODO: Adapt this
	virtual void OnRenderScene() {}

	void ProcessScroll(double yoffset);
	void ProcessMouseMovement(double xoffset, double yoffset);
	void ProcessInput(CameraMovement movement, float dt);

	virtual void LoadModel(const char* path);

private:

public:
	glm::vec4 backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);

protected:
	R_Material* defMaterial = nullptr;
	std::vector<R_Model*> models;
	// TODO: Add lights

private:
	Camera* cameras[MAX_CAMERAS];
	Camera* activeCamera = nullptr;

	int viewportWidth, viewportHeight;
};

#endif // !__R_SCENE_H__