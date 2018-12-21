#ifndef __R_GEOMETRY_H__
#define __R_GEOMETRY_H__

#include "Resource.h"
#include <GL/glew.h>

class R_Geometry : public Resource
{
	friend class ModelLoader;
public:
	R_Geometry(const char* name);
	R_Geometry(const char* name, unsigned int numVertices, unsigned int numIndices, int* indices, float* vertices, float* normals = nullptr, float* texCoords = nullptr, float* colors = nullptr);
	virtual ~R_Geometry();

	void Set(unsigned int numVertices, unsigned int numIndices, int* indices, float* vertices, float* normals = nullptr, float* texCoords = nullptr, float* colors = nullptr);

	void SetVertices(float* ptr, int verticesCount); //Use with same vertices size
	void SetIndices(int* ptr, int indicesCount); //Use with same indices size
	void SetNormals(float* ptr, int verticesCount); //Use with same vertices size
	void SetTexCoords(float* ptr, int verticesCount); //Use with same vertices size
	void SetVertexColors(float* ptr, int verticesCount); //Use with same vertices size

	void SendInfoToVRAM();

	void Free() override;
	void FreeVRam();

	int CountVertices()const { return m_numVertices; }
	int CountIndices()const { return m_numIndices; }
	GLuint EBO()const { return m_idContainer; }
	GLuint IdIndices()const { return m_idIndices; }
	GLuint IdVertices()const { return m_idVertices; }
	GLuint IdNormals()const { return m_idNormals; }
	GLuint IdUvs()const { return m_idTexCoords; }
	GLuint IdColors()const { return m_idColors; }

private:

	float* m_vertices = nullptr;
	int* m_indices = nullptr;
	float* m_normals = nullptr;
	float* m_texCoords = nullptr;
	float* m_colors = nullptr;

	int m_numVertices = 0;
	int m_numIndices = 0;

	GLuint m_idContainer = 0;

	GLuint m_idIndices = 0;
	GLuint m_idVertices = 0;
	GLuint m_idNormals = 0;
	GLuint m_idTexCoords = 0;
	GLuint m_idColors = 0;
};

#endif // !__R_GEOMETRY_H__