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

	int Width()const { return m_viewportWidth; }
	int Height()const { return m_viewportHeight; }

	R_Material* GetDefaultMaterial()const;

	void RenderScene(); // TODO: Adapt this
	virtual void OnRenderScene() {}

	void ProcessScroll(double yoffset);
	void ProcessMouseMovement(double xoffset, double yoffset);
	void ProcessInput(CameraMovement movement, float dt);

	std::vector<R_Model*>* GetModels() { return &m_models; }

private:

public:
	glm::vec4 m_backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);

protected:
	R_Material* m_defMaterial = nullptr;
	std::vector<R_Model*> m_models;
	// TODO: Add lights

private:
	Camera* m_cameras[MAX_CAMERAS];
	Camera* m_activeCamera = nullptr;

	int m_viewportWidth, m_viewportHeight;
};

#endif // !__R_SCENE_H__