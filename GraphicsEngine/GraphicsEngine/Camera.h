#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Defs.h"

enum CameraType
{
	CAM_PERSPECTIVE = 0,
	CAM_ORTHOGRAPHIC,
	CAM_NONE
};

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	// Eular Angles
	float m_Yaw;
	float m_Pitch;
	// Camera options
	float m_MovementSpeed;
	float m_MouseSensitivity;
	
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	virtual ~Camera() = default;

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	// Returns the projection matrix calculated
	virtual glm::mat4 GetProjectionMatrix();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(CameraMovement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

	void ResizeViewport(int width, int height);

	float GetZNear()const;
	void SetZNear(float n);

	float GetZFar()const;
	void SetZFar(float f);

	float GetZoom()const;

	float GetSize()const;
	void SetSize(float s);

	CameraType GetCameraType()const;
	// TODO: Let change the camera type

protected:
	virtual void CalcProjection() = 0;
	virtual void OnViewportResize() = 0;

	glm::mat4 m_view = glm::mat4(1.0f);
	glm::mat4 m_projection = glm::mat4(1.0f);

	int m_width = DEFAULT_WIN_WIDTH, m_height = DEFAULT_WIN_HEIGHT;
	float m_zNear = 1.0f, m_zFar = 100.0f;

	float m_Zoom;
	float m_Size = 10.0f;

	CameraType m_cameraType = CAM_NONE;

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void UpdateCameraVectors(bool forceCalcProj);
};


#endif // !__CAMERA_H__