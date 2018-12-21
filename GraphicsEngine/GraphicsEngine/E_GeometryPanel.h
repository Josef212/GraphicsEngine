#ifndef __E_GEOMETRYPANEL_H__
#define __E_GEOMETRYPANEL_H__

#include "Panel.h"

class R_Geometry;

class E_GeometryPanel : public Panel
{
public:
	E_GeometryPanel();
	virtual ~E_GeometryPanel();

	void OnInit() override;
	void Display() override;

private:
	void GeometryInfo(R_Geometry* geo);
	void GeometryRender();

private:
	R_Geometry* m_selectedGeo = nullptr;
	// TODO: A framebuffer to preview the geometry
};

#endif // !__E_GEOMETRYPANEL_H__