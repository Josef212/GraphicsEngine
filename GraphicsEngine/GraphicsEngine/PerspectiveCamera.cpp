#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>


PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Camera(position, up, yaw, pitch)
{
	m_cameraType = CAM_PERSPECTIVE;
	CalcProjection();
}

PerspectiveCamera::PerspectiveCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Camera(posX, posY, posZ, upX, upY, upZ, yaw, pitch)
{
	m_cameraType = CAM_PERSPECTIVE;
	CalcProjection();
}

PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::CalcProjection()
{
	m_projection = glm::perspective(glm::radians(m_Zoom), (float)m_width / (float)m_height, m_zNear, m_zFar);
}

void PerspectiveCamera::OnViewportResize()
{
	CalcProjection();
}
