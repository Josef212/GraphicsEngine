#include "R_Geometry.h"

#include "Defs.h"
#include <cstring>


R_Geometry::R_Geometry(const char* name) : Resource(name, RES_GEOMETRY)
{
}

R_Geometry::R_Geometry(const char* name, unsigned int numVertices, unsigned int numIndices, int * indices, float * vertices, float * normals, float * texCoords, float * colors) : Resource(name, RES_GEOMETRY)
{
	this->numIndices = numIndices;
	this->numVertices = numVertices;

	SetIndices(indices, numIndices);
	SetVertices(vertices, numVertices);
	SetNormals(normals, numVertices);
	SetTexCoords(texCoords, numVertices);
	SetVertexColors(colors, numVertices);

	SendInfoToVRAM();
}


R_Geometry::~R_Geometry()
{}

void R_Geometry::Set(unsigned int numVertices, unsigned int numIndices, int * indices, float * vertices, float * normals, float * texCoords, float * colors)
{
	Free();

	this->numIndices = numIndices;
	this->numVertices = numVertices;

	SetIndices(indices, numIndices);
	SetVertices(vertices, numVertices);
	SetNormals(normals, numVertices);
	SetTexCoords(texCoords, numVertices);
	SetVertexColors(colors, numVertices);

	SendInfoToVRAM();
}

void R_Geometry::SetVertices(float* ptr, int verticesCount)
{
	if (!ptr) return;

	RELEASE_ARRAY(vertices);

	vertices = new float[verticesCount * 3];
	memcpy(vertices, ptr, sizeof(float) * verticesCount * 3);
}

void R_Geometry::SetIndices(int* ptr, int indicesCount)
{
	if (!ptr) return;

	RELEASE_ARRAY(indices);

	indices = new int[indicesCount];
	memcpy(indices, ptr, sizeof(int) * indicesCount);
}

void R_Geometry::SetNormals(float* ptr, int verticesCount)
{
	if (!ptr) return;

	RELEASE_ARRAY(normals);

	normals = new float[verticesCount * 3];
	memcpy(normals, ptr, sizeof(float) * verticesCount * 3);
}

void R_Geometry::SetTexCoords(float* ptr, int verticesCount)
{
	if (!ptr) return;

	RELEASE_ARRAY(texCoords);

	texCoords = new float[verticesCount * 2];
	memcpy(texCoords, ptr, sizeof(float) * verticesCount * 2);
}

void R_Geometry::SetVertexColors(float* ptr, int verticesCount)
{
	if (!ptr) return;

	RELEASE_ARRAY(colors);

	colors = new float[verticesCount * 3];
	memcpy(colors, ptr, sizeof(float) * verticesCount * 3);
}

void R_Geometry::SendInfoToVRAM()
{
	if (vertices && indices)
	{
		glGenVertexArrays(1, &idContainer);

		glGenBuffers(1, &idVertices);
		glGenBuffers(1, &idIndices);
		if (normals) glGenBuffers(1, &idNormals);
		if (texCoords) glGenBuffers(1, &idTexCoords);
		if (colors) glGenBuffers(1, &idColors);

		glBindVertexArray(idContainer);

		glBindBuffer(GL_ARRAY_BUFFER, idVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numIndices, indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		if (normals)
		{
			glBindBuffer(GL_ARRAY_BUFFER, idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, normals, GL_STATIC_DRAW);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
			glEnableVertexAttribArray(1);
		}

		if (texCoords)
		{
			glBindBuffer(GL_ARRAY_BUFFER, idTexCoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 2, texCoords, GL_STATIC_DRAW);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
			glEnableVertexAttribArray(2);
		}

		if (colors)
		{
			glBindBuffer(GL_ARRAY_BUFFER, idColors);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, colors, GL_STATIC_DRAW);

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
			glEnableVertexAttribArray(3);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void R_Geometry::Free()
{
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(texCoords);
	RELEASE_ARRAY(colors);

	numIndices = 0;
	numVertices = 0;

	FreeVRam();
}

void R_Geometry::FreeVRam()
{
	if (idIndices > 0) { glDeleteBuffers(1, &idIndices); idIndices = 0; }
	if (idVertices > 0) { glDeleteBuffers(1, &idVertices); idVertices = 0; }
	if (idNormals > 0) { glDeleteBuffers(1, &idNormals); idNormals = 0; }
	if (idTexCoords > 0) { glDeleteBuffers(1, &idTexCoords); idTexCoords = 0; }
	if (idColors > 0) { glDeleteBuffers(1, &idColors); idColors = 0; }

	if (idContainer > 0) { glDeleteVertexArrays(1, &idContainer); idContainer = 0; }
}
