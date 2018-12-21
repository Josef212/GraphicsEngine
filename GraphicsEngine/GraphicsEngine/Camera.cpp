#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVTY), m_Zoom(ZOOM)
{
	m_Position = position;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateCameraVectors(false);
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVTY), m_Zoom(ZOOM)
{
	m_Position = glm::vec3(posX, posY, posZ);
	m_WorldUp = glm::vec3(upX, upY, upZ);
	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateCameraVectors(false);
}

glm::mat4 Camera::GetViewMatrix()
{
	return m_view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return m_projection;
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;
	if (direction == FORWARD)
		m_Position += m_Front * velocity;
	if (direction == BACKWARD)
		m_Position -= m_Front * velocity;
	if (direction == LEFT)
		m_Position -= m_Right * velocity;
	if (direction == RIGHT)
		m_Position += m_Right * velocity;
	if (direction == UP)
		m_Position += m_WorldUp * velocity;
	if (direction == DOWN)
		m_Position -= m_WorldUp * velocity;

	m_view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	UpdateCameraVectors(false);
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= yoffset;
	if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom >= 45.0f)
		m_Zoom = 45.0f;

	CalcProjection();
}

void Camera::ResizeViewport(int width, int height)
{
	this->m_width = width;
	this->m_height = height;

	OnViewportResize();
}

float Camera::GetZNear() const
{
	return m_zNear;
}

void Camera::SetZNear(float n)
{
	if(n < m_zFar)
	{
		m_zNear = n;
		CalcProjection();
	}
}

float Camera::GetZFar() const
{
	return m_zFar;
}

void Camera::SetZFar(float f)
{
	if(f > m_zNear)
	{
		m_zFar = f;
		CalcProjection();
	}
}

float Camera::GetZoom() const
{
	return m_Zoom;
}

float Camera::GetSize() const
{
	return m_Size;
}

void Camera::SetSize(float s)
{
	m_Size = s;
}

CameraType Camera::GetCameraType() const
{
	return m_cameraType;
}

// ------

void Camera::UpdateCameraVectors(bool forceCalcProj)
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

	m_view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);

	if (forceCalcProj)
		CalcProjection();
}