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

class Light;

enum CameraMovement;

#define MAX_CAMERAS 9

class R_Scene : public Resource
{
	friend class R_Renderer;
public:
	R_Scene(const char* name);
	virtual ~R_Scene();

	void Load() override;
	void Free() override;

	virtual void OnLoad() {}
	virtual void OnFree() {}

	virtual void OnActiveCameraChanged() {}

	// Models
	void AddModel(R_Model* model);
	void RemoveModel(R_Model* model);
	void RemoveModel(const char* name);

	std::vector<R_Model*>* GetModels() { return &m_models; }
	int GetModelsCount() const { return m_models.size(); }
	inline R_Model* GetModel(int index) const
	{
		if (index >= 0 && index < GetModelsCount())
		{
			return m_models[index];
		}

		return nullptr;
	}

	// Cameras
	void AddCamera(Camera* cam, int index, bool setAsActive = false);
	void RemoveCamera(int index);
	void SetActiveCamera(int index);
	Camera* GetActiveCamera() const;

	void ProcessScroll(double yoffset) const;
	void ProcessMouseMovement(double xoffset, double yoffset) const;
	void ProcessInput(CameraMovement movement, float dt) const;

	void OnResize(int w, int h);

	int Width()const { return m_viewportWidth; }
	int Height()const { return m_viewportHeight; }

	R_Material* GetDefaultMaterial()const;

	// Lights
	void AddLight(Light* light);
	void RemoveLight(Light* light);
	void RemoveLight(int index);
	std::vector<Light*>* GetLights() { return &m_lights; }
	int GetLightsCount() const { return m_lights.size(); }
	inline Light* GetLight(int index) const
	{
		if(index >= 0 && index < m_lights.size())
		{
			return m_lights[index];
		}

		return nullptr;
	}

private:

public:
	glm::vec4 m_backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);

protected:
	R_Material* m_defMaterial = nullptr;
	std::vector<R_Model*> m_models;
	std::vector<Light*> m_lights;

private:
	Camera* m_cameras[MAX_CAMERAS];
	Camera* m_activeCamera = nullptr;

	int m_viewportWidth, m_viewportHeight;
};

#endif // !__R_SCENE_H__