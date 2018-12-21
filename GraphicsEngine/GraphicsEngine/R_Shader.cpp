#include "R_Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>



R_Shader::R_Shader(const char* name) : Resource(name, RES_SHADER)
{
	m_vertPath = "";
	m_fragPath = "";
	m_geoPath = "";
}

R_Shader::R_Shader(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath) : Resource(name, RES_SHADER)
{
	m_vertPath = vertexPath;
	m_fragPath = fragmentPath;
	m_geoPath = geometryPath ? geometryPath : "";

	LoadShader();
}

R_Shader::~R_Shader()
{
}

void R_Shader::LoadShader(const char * vertexPath, const char * fragmentPath, const char * geometryPath)
{
	m_vertPath = vertexPath;
	m_fragPath = fragmentPath;
	m_geoPath = geometryPath;

	LoadShader();
}

void R_Shader::LoadShader()
{
	std::string vertexCode = LoadCode(m_vertPath.c_str());
	std::string fragmentCode = LoadCode(m_fragPath.c_str());
	std::string geometryCode;
	if (!m_geoPath.empty()) geometryCode = LoadCode(m_geoPath.c_str());

	unsigned int vertex, fragment, geometry;

	vertex = CreateShader(vertexCode.c_str(), GL_VERTEX_SHADER);
	fragment = CreateShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
	if (!m_geoPath.empty()) geometry = CreateShader(geometryCode.c_str(), GL_GEOMETRY_SHADER);

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	if (!m_geoPath.empty()) glAttachShader(m_ID, geometry);

	glLinkProgram(m_ID);
	CheckCompileErrors(m_ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!m_geoPath.empty()) glDeleteShader(geometry);
}

void R_Shader::ReloadShader()
{
	glDeleteProgram(m_ID);
	LoadShader();
}

void R_Shader::Load()
{
	if (!m_vertPath.empty() && !m_fragPath.empty()) LoadShader();
}

void R_Shader::Free()
{
	glDeleteProgram(m_ID);
	m_vertPath = m_fragPath = m_geoPath = "";
	m_status = SH_PENDING;
}

void R_Shader::Use()const
{
	glUseProgram(m_ID);
}

// -----

void R_Shader::SetBool(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name), (int)value);
}
void R_Shader::SetInt(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name), value);
}
void R_Shader::SetFloat(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ID, name), value);
}
void R_Shader::SetVec2(const char* name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_ID, name), 1, &value[0]);
}
void R_Shader::SetVec2(const char* name, const float* value) const
{
	glUniform2fv(glGetUniformLocation(m_ID, name), 1, value);
}
void R_Shader::SetVec2(const char* name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_ID, name), x, y);
}
void R_Shader::SetVec3(const char* name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_ID, name), 1, &value[0]);
}
void R_Shader::SetVec3(const char* name, const float* value) const
{
	glUniform3fv(glGetUniformLocation(m_ID, name), 1, value);
}
void R_Shader::SetVec3(const char* name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_ID, name), x, y, z);
}
void R_Shader::SetVec4(const char* name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_ID, name), 1, &value[0]);
}
void R_Shader::SetVec4(const char* name, const float* value) const
{
	glUniform4fv(glGetUniformLocation(m_ID, name), 1, value);
}
void R_Shader::SetVec4(const char* name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_ID, name), x, y, z, w);
}
void R_Shader::SetMat2(const char* name, const glm::mat2& value) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, &value[0][0]);
}
void R_Shader::SetMat2(const char* name, const float* value) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, value);
}
void R_Shader::SetMat3(const char* name, const glm::mat3& value) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, &value[0][0]);
}
void R_Shader::SetMat3(const char* name, const float* value) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, value);
}
void R_Shader::SetMat4(const char* name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, &value[0][0]);
}
void R_Shader::SetMat4(const char* name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, value);
}

void R_Shader::LogCode()
{
	std::string vertexCode = LoadCode(m_vertPath.c_str());
	std::string fragmentCode = LoadCode(m_fragPath.c_str());
	std::string geometryCode;
	if (!m_geoPath.empty()) geometryCode = LoadCode(m_geoPath.c_str());

	std::cout << "==========================" << std::endl;

	std::cout << "SHADER CODE: " << GetName() << std::endl;
	std::cout << "STATUS: " << GetStatusStr() << std::endl;

	std::cout << "\tVertex: " << std::endl;
	std::cout << vertexCode << std::endl << std::endl;
	std::cout << "\tFragment: " << std::endl;
	std::cout << fragmentCode << std::endl << std::endl;
	std::cout << "\tGeometry: " << std::endl;
	std::cout << geometryCode << std::endl << std::endl;

	std::cout << "==========================" << std::endl;
}

Shader_Status R_Shader::GetStatus() const
{
	return m_status;
}

const char* R_Shader::GetStatusStr() const
{
	static const char* statusStr[] = {
		"COMPILED", "ERROR", "PENDING"
		};

	return statusStr[m_status];
}

// ---------------------------------------------------------------------
// TODO: Use FS to load the code
std::string R_Shader::LoadCode(const char* path)
{
	std::string ret;
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		ret = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ from [" << path << "]." << std::endl;
	}

	return ret;
}

int R_Shader::CreateShader(const char* code, GLenum type)
{
	int ret = 0;

	ret = glCreateShader(type);
	glShaderSource(ret, 1, &code, NULL);
	glCompileShader(ret);
	CheckCompileErrors(ret, "SHADER");

	return ret;
}

void R_Shader::CheckCompileErrors(int sh, const char* type)
{
	int success;
	char infoLog[1024];
	if (strcmp(type, "PROGRAM") != 0)
	{
		glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(sh, 1024, NULL, infoLog);
			std::cout << GetName() << "->" << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(sh, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(sh, 1024, NULL, infoLog);
			std::cout << GetName() << "->" << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			m_status = SH_ERROR;
		}
		else
		{
			m_status = SH_COMPILED;
			std::cout << GetName() << " just compiled." << std::endl;
		}
	}
}