#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>


OrthographicCamera::OrthographicCamera()
{
	m_cameraType = CAM_ORTHOGRAPHIC;
	CalcProjection();
}


OrthographicCamera::~OrthographicCamera()
{

}

void OrthographicCamera::CalcProjection()
{
	m_projection = glm::ortho(-m_Size, m_Size, -m_Size, m_Size, 1.0f, 100.0f);
}

void OrthographicCamera::OnViewportResize()
{
}
