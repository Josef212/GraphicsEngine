#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>


PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Camera(position, up, yaw, pitch)
{
	cameraType = CAM_PERSPECTIVE;
	CalcProjection();
}

PerspectiveCamera::PerspectiveCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Camera(posX, posY, posZ, upX, upY, upZ, yaw, pitch)
{
	cameraType = CAM_PERSPECTIVE;
	CalcProjection();
}

PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::CalcProjection()
{
	projection = glm::perspective(glm::radians(Zoom), (float)width / (float)height, zNear, zFar);
}

void PerspectiveCamera::OnViewportResize()
{
	CalcProjection();
}
