#ifndef __PERSPECTIVECAMERA_H__
#define __PERSPECTIVECAMERA_H__

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	PerspectiveCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	virtual ~PerspectiveCamera();

	void CalcProjection() override;
	void OnViewportResize() override;
};

#endif // !__PERSPECTIVECAMERA_H__