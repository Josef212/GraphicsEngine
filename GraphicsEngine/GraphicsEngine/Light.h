#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glm/glm.hpp>

enum LightType
{
	L_POINT = 0,
	L_DIRECTIONAL = 1,
	L_SPOTLIGHT = 2
};

class Light
{
public:
	Light();
	virtual ~Light();

	glm::vec3 m_position;
	glm::vec3 m_color;

	LightType m_type = LightType::L_POINT;
};

#endif // !__LIGHT_H__