#ifndef __R_MATERIAL_H__
#define __R_MATERIAL_H__

#include "Resource.h"

class R_Shader;
class R_Scene;

class R_Material : public Resource
{
public:
	R_Material(const char* name);
	R_Material(const char* name, R_Shader* sh);
	virtual ~R_Material();

	void Load() override;
	void Free() override;

	void InitRender()const;
	void EndRender()const;

	virtual void SendInfo(R_Scene* scene)const {}
	virtual void OnLoad() {}
	virtual void OnFree() {}

	void SetShader(R_Shader* sh) { m_shader = sh; }
	R_Shader* GetShader()const { return m_shader; }

	bool ValidMaterial() const { return m_shader; }

	virtual void OnEditUI() {}

protected:
	R_Shader * m_shader = nullptr;
};

#endif // !__R_MATERIAL_H__