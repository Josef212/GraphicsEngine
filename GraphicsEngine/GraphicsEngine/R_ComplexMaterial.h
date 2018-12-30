#ifndef __R_COMPLEXMATERIAL_H__
#define __R_COMPLEXMATERIAL_H__

#include "R_Material.h"
#include "Defs.h"
#include <vector>
#include <glm/glm.hpp>

class R_Texture;

enum MatPropertyValueType
{
	MAT_INT = 0,
	MAT_INT_PTR,
	MAT_FLOAT,
	MAT_FLOAT_PTR,
	MAT_BOOL,
	MAT_BOOL_PTR,
	MAT_TEXTURE,
	MAT_VEC2,
	MAT_VEC3,
	MAT_VEC4,
	MAT_MAT3,
	MAT_MAT4
};

const char* GetMatPropertyValueTypeStr(MatPropertyValueType type);

class MatProperty
{
public:
	MatProperty(const char* name, MatPropertyValueType type);
	MatProperty(const char* name, int& value);
	MatProperty(const char* name, int* value);
	MatProperty(const char* name, float& value);
	MatProperty(const char* name, float* value);
	MatProperty(const char* name, bool& value);
	MatProperty(const char* name, bool* value);
	MatProperty(const char* name, R_Texture* value);
	MatProperty(const char* name, glm::vec2& value);
	MatProperty(const char* name, glm::vec3& value);
	MatProperty(const char* name, glm::vec4& value);
	MatProperty(const char* name, glm::mat3& value);
	MatProperty(const char* name, glm::mat4& value);

public:
	union
	{
		int _int;
		int* _intPtr;
		float _float;
		float* _floatPtr;
		bool _bool;
		bool* _boolPtr;
		R_Texture* _texture;
		glm::vec2 _vec2;
		glm::vec3 _vec3;
		glm::vec4 _vec4;
		glm::mat3 _mat3;
		glm::mat4 _mat4;
	}m_propertyValue;

	std::string m_propertyName;
	MatPropertyValueType m_propertyType;
	
};

class R_ComplexMaterial : public R_Material
{
public:
	R_ComplexMaterial(const char* name);
	R_ComplexMaterial(const char* name, R_Shader* sh);
	virtual ~R_ComplexMaterial();

	void AddProperty(MatProperty* prop);
	void RemoveProperty(MatProperty* prop);
	void RemoveProperty(const char* name);
	MatProperty* GetProperty(const char* name);
	MatProperty* GetProperty(int index);

	int GetPropertyCount() const { return m_properties.size(); }

	void OnFree() override;
	void SendInfo(R_Scene* scene) const override;
	void OnEditUI() override;


protected:
	std::vector<MatProperty*> m_properties;
};

#endif // !__R_COMPLEXMATERIAL_H__