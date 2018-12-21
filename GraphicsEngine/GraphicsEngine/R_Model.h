#ifndef __R_MODEL_H__
#define __R_MODEL_H__

#include "Resource.h"

#include <vector>
#include <string>

#include <glm/glm.hpp>

#define NO_RENDER_CONFIG 0
#define PASS_MODEL 1
#define PASS_VIEW 2
#define PASS_PROJ 4
#define PASS_PROJ_VIEW_MODEL  PASS_MODEL | PASS_VIEW | PASS_PROJ

class R_Geometry;
class R_Material;

class R_Model : public Resource
{
public:
	R_Model(const char* name);
	R_Model(const char* name, R_Geometry* geo, R_Material* mat);
	virtual ~R_Model();

	void Free() override;

	void AddMesh(R_Geometry* geo, R_Material* mat);
	void SetGeometry(R_Geometry* geo, int index);
	void SetMaterial(R_Material* mat, int index);
	void SetMaterial(R_Material* mat);

	std::pair<R_Geometry*, R_Material*> GetMesh(int index)const;
	R_Geometry* GetGeometry(int index)const;
	R_Material* GetMaterial(int index)const;

	void Render(int renderConfig = PASS_PROJ_VIEW_MODEL);

	glm::vec3 GetTranslation()const;
	void SetTranslation(glm::vec3 pos);

	glm::vec3 GetEuler()const;
	void SetEuler(glm::vec3 euler);

	glm::vec3 GetScale()const;
	void SetScale(glm::vec3 scl);

private:
	void CalcModel();

public:
	glm::mat4 m_modelMat;
	std::string m_directory = "";

private:
	glm::vec3 m_translation = glm::vec3(0.f);
	glm::vec3 m_eulerRot = glm::vec3(0.f);
	glm::vec3 m_scale = glm::vec3(1.f);

	std::vector<std::pair<R_Geometry*, R_Material*>> m_meshes;
};

#endif // !__R_MODEL_H__