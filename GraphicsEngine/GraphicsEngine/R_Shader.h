#ifndef __SHADER_H__
#define __SHADER_H__

#include "Resource.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

enum Shader_Status
{
	SH_COMPILED = 0,
	SH_ERROR,
	SH_PENDING
};

class R_Shader : public Resource
{
public:
	R_Shader(const char* name);
	R_Shader(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	virtual ~R_Shader();

	void LoadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void LoadShader();
	void ReloadShader();

	void Load() override;
	void Free() override;

	void Use()const;

	void SetBool(const char* name, bool value) const;
	void SetInt(const char* name, int value) const;
	void SetFloat(const char* name, float value) const;
	void SetVec2(const char* name, const glm::vec2& value) const;
	void SetVec2(const char* name, const float* value) const;
	void SetVec2(const char* name, float x, float y) const;
	void SetVec3(const char* name, const glm::vec3& value) const;
	void SetVec3(const char* name, const float* value) const;
	void SetVec3(const char* name, float x, float y, float z) const;
	void SetVec4(const char* name, const glm::vec4& value) const;
	void SetVec4(const char* name, const float* value) const;
	void SetVec4(const char* name, float x, float y, float z, float w) const;
	void SetMat2(const char* name, const glm::mat2& value) const;
	void SetMat2(const char* name, const float* value) const;
	void SetMat3(const char* name, const glm::mat3& value) const;
	void SetMat3(const char* name, const float* value) const;
	void SetMat4(const char* name, const glm::mat4& value) const;
	void SetMat4(const char* name, const float* value) const;

	void LogCode();

	Shader_Status GetStatus()const;
	const char* GetStatusStr()const;

private:
	std::string LoadCode(const char* path);
	int CreateShader(const char* code, GLenum type);
	void CheckCompileErrors(int sh, const char* type);

public:
	unsigned int m_ID;
	std::string m_vertPath, m_fragPath, m_geoPath;

private:
	Shader_Status m_status = SH_PENDING;
};

#endif // !__SHADER_H__