#ifndef __ORTHOGRAPHICCAMERA_H__
#define __ORTHOGRAPHICCAMERA_H__

#include "Camera.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera();
	virtual ~OrthographicCamera();

	void CalcProjection() override; 
	void OnViewportResize() override;
};

#endif // !__ORTHOGRAPHICCAMERA_H__