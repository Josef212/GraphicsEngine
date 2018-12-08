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

	int CountVertices()const { return numVertices; }
	int CountIndices()const { return numIndices; }
	GLuint EBO()const { return idContainer; }
	GLuint IdIndices()const { return idIndices; }
	GLuint IdVertices()const { return idVertices; }
	GLuint IdNormals()const { return idNormals; }
	GLuint IdUvs()const { return idTexCoords; }
	GLuint IdColors()const { return idColors; }

private:

	float* vertices = nullptr;
	int* indices = nullptr;
	float* normals = nullptr;
	float* texCoords = nullptr;
	float* colors = nullptr;

	int numVertices = 0;
	int numIndices = 0;

	GLuint idContainer = 0;

	GLuint idIndices = 0;
	GLuint idVertices = 0;
	GLuint idNormals = 0;
	GLuint idTexCoords = 0;
	GLuint idColors = 0;
};

#endif // !__R_GEOMETRY_H__