#include "R_ComplexMaterial.h"

#include "R_Shader.h"
#include "R_Texture.h"
#include "OpenGL.h"


const char* GetMatPropertyValueTypeStr(MatPropertyValueType type)
{
	static const char* rets[] = {
		"INT", "INT_PTR", "FLOAT", "FLOAT_PTR", "BOOL", "BOOL_PTR", "TEXTURE", 
		"VEC2", "VEC3", "VEC4", "MAT3", "MAT4"
	};

	return rets[type];
}

MatProperty::MatProperty(const char * name, int & value) : m_propertyName(name), m_propertyType(MAT_INT)
{
	m_propertyValue._int = value;
}

MatProperty::MatProperty(const char * name, int * value) : m_propertyName(name), m_propertyType(MAT_INT_PTR)
{
	m_propertyValue._intPtr = value;
}

MatProperty::MatProperty(const char * name, float & value) : m_propertyName(name), m_propertyType(MAT_FLOAT)
{
	m_propertyValue._float = value;
}

MatProperty::MatProperty(const char * name, float * value) : m_propertyName(name), m_propertyType(MAT_FLOAT_PTR)
{
	m_propertyValue._floatPtr = value;
}

MatProperty::MatProperty(const char * name, bool & value) : m_propertyName(name), m_propertyType(MAT_BOOL)
{
	m_propertyValue._bool = value;
}

MatProperty::MatProperty(const char * name, bool * value) : m_propertyName(name), m_propertyType(MAT_BOOL_PTR)
{
	m_propertyValue._boolPtr = value;
}

MatProperty::MatProperty(const char * name, R_Texture * value) : m_propertyName(name), m_propertyType(MAT_TEXTURE)
{
	m_propertyValue._texture = value;
}

MatProperty::MatProperty(const char * name, glm::vec2 & value) : m_propertyName(name), m_propertyType(MAT_VEC2)
{
	m_propertyValue._vec2 = value;
}

MatProperty::MatProperty(const char * name, glm::vec3 & value) : m_propertyName(name), m_propertyType(MAT_VEC3)
{
	m_propertyValue._vec3 = value;
}

MatProperty::MatProperty(const char * name, glm::vec4 & value) : m_propertyName(name), m_propertyType(MAT_VEC4)
{
	m_propertyValue._vec4 = value;
}

MatProperty::MatProperty(const char * name, glm::mat3 & value) : m_propertyName(name), m_propertyType(MAT_MAT3)
{
	m_propertyValue._mat3 = value;
}

MatProperty::MatProperty(const char * name, glm::mat4 & value) : m_propertyName(name), m_propertyType(MAT_MAT4)
{
	m_propertyValue._mat4 = value;
}

// ======================================================================

R_ComplexMaterial::R_ComplexMaterial(const char* name) : R_Material(name)
{
}

R_ComplexMaterial::R_ComplexMaterial(const char* name, R_Shader* sh) : R_Material(name, sh)
{
}


R_ComplexMaterial::~R_ComplexMaterial()
{
	OnFree();
}

void R_ComplexMaterial::AddProperty(MatProperty * prop)
{
	if (prop) m_properties.push_back(prop);
}

void R_ComplexMaterial::RemoveProperty(MatProperty * prop)
{
	if(prop)
	{
		auto p = std::find(m_properties.begin(), m_properties.end(), prop);
		if(p != m_properties.end())
		{
			RELEASE((*p));
			m_properties.erase(p);
		}
	}
}

void R_ComplexMaterial::RemoveProperty(const char * name)
{
	if (!name) return;

	for(auto it = m_properties.begin(); it != m_properties.end();)
	{
		if((*it)->m_propertyName == name)
		{
			RELEASE((*it));
			it = m_properties.erase(it);
		}
		else
		{
			++it;
		}
	}
}

MatProperty * R_ComplexMaterial::GetProperty(const char * name)
{
	if(name)
	{
		for (auto it = m_properties.begin(); it != m_properties.end();)
		{
			if ((*it)->m_propertyName == name)
			{
				return (*it);
			}
			else
			{
				++it;
			}
		}
	}

	return nullptr;
}

MatProperty * R_ComplexMaterial::GetProperty(int index)
{
	if (index >= 0 && index < m_properties.size())
		return m_properties[index];

	return nullptr;
}

void R_ComplexMaterial::OnFree()
{
	for (auto it = m_properties.begin(); it != m_properties.end();)
	{
		RELEASE((*it));
		it = m_properties.erase(it);
	}
}

void R_ComplexMaterial::SendInfo(R_Scene * scene) const
{
	int texCount = 0;

	for(auto it : m_properties)
	{
		if(it)
		{
			switch (it->m_propertyType)
			{
			case MatPropertyValueType::MAT_INT:
				m_shader->SetInt(it->m_propertyName.c_str(), it->m_propertyValue._int);
				break;

			case MatPropertyValueType::MAT_INT_PTR:
				m_shader->SetInt(it->m_propertyName.c_str(), *it->m_propertyValue._intPtr);
				break;

			case MatPropertyValueType::MAT_FLOAT:
				m_shader->SetFloat(it->m_propertyName.c_str(), it->m_propertyValue._float);
				break;

			case MatPropertyValueType::MAT_FLOAT_PTR:
				m_shader->SetFloat(it->m_propertyName.c_str(), *it->m_propertyValue._floatPtr);
				break;

			case MatPropertyValueType::MAT_BOOL:
				m_shader->SetBool(it->m_propertyName.c_str(), it->m_propertyValue._bool);
				break;

			case MatPropertyValueType::MAT_BOOL_PTR:
				m_shader->SetBool(it->m_propertyName.c_str(), *it->m_propertyValue._boolPtr);
				break;

			case MatPropertyValueType::MAT_VEC2:
				m_shader->SetVec2(it->m_propertyName.c_str(), it->m_propertyValue._vec2);
				break;

			case MatPropertyValueType::MAT_VEC3:
				m_shader->SetVec3(it->m_propertyName.c_str(), it->m_propertyValue._vec3);
				break;

			case MatPropertyValueType::MAT_VEC4:
				m_shader->SetVec4(it->m_propertyName.c_str(), it->m_propertyValue._vec4);
				break;

			case MatPropertyValueType::MAT_MAT3:
				m_shader->SetMat3(it->m_propertyName.c_str(), it->m_propertyValue._mat3);
				break;

			case MatPropertyValueType::MAT_MAT4:
				m_shader->SetMat4(it->m_propertyName.c_str(), it->m_propertyValue._mat4);
				break;

			case MatPropertyValueType::MAT_TEXTURE:
				glActiveTexture(GL_TEXTURE0 + texCount);
				glBindTexture(GL_TEXTURE_2D, it->m_propertyValue._texture->TextureID());
				m_shader->SetInt(it->m_propertyName.c_str(), texCount);
				++texCount;
				break;
			}
		}
	}
}
