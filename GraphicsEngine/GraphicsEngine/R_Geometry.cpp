#include "R_Geometry.h"

#include "Defs.h"
#include <cstring>


R_Geometry::R_Geometry(const char* name) : Resource(name, RES_GEOMETRY)
{
}

R_Geometry::R_Geometry(const char* name, unsigned int numVertices, unsigned int numIndices, int * indices, float * vertices, float * normals, float * texCoords, float * colors) : Resource(name, RES_GEOMETRY)
{
	this->m_numIndices = numIndices;
	this->m_numVertices = numVertices;

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

	this->m_numIndices = numIndices;
	this->m_numVertices = numVertices;

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

	RELEASE_ARRAY(m_vertices);

	m_vertices = new float[verticesCount * 3];
	memcpy(m_vertices, ptr, sizeof(float) * verticesCount * 3);
}

void R_Geometry::SetIndices(int* ptr, int indicesCount)
{
	if (!ptr) return;

	RELEASE_ARRAY(m_indices);

	m_indices = new int[indicesCount];
	memcpy(m_indices, ptr, sizeof(int) * indicesCount);
}

void R_Geometry::SetNormals(float* ptr, int verticesCount)
{
	if (!ptr) return;

	RELEASE_ARRAY(m_normals);

	m_normals = new float[verticesCount * 3];
	memcpy(m_normals, ptr, sizeof(float) * verticesCount * 3);
}

void R_Geometry::SetTexCoords(float* ptr, int verticesCount)
{
	if (!ptr) return;

	RELEASE_ARRAY(m_texCoords);

	m_texCoords = new float[verticesCount * 2];
	memcpy(m_texCoords, ptr, sizeof(float) * verticesCount * 2);
}

void R_Geometry::SetVertexColors(float* ptr, int verticesCount)
{
	if (!ptr) return;

	RELEASE_ARRAY(m_colors);

	m_colors = new float[verticesCount * 3];
	memcpy(m_colors, ptr, sizeof(float) * verticesCount * 3);
}

void R_Geometry::SendInfoToVRAM()
{
	if (m_vertices && m_indices)
	{
		glGenVertexArrays(1, &m_idContainer);

		glGenBuffers(1, &m_idVertices);
		glGenBuffers(1, &m_idIndices);
		if (m_normals) glGenBuffers(1, &m_idNormals);
		if (m_texCoords) glGenBuffers(1, &m_idTexCoords);
		if (m_colors) glGenBuffers(1, &m_idColors);

		glBindVertexArray(m_idContainer);

		glBindBuffer(GL_ARRAY_BUFFER, m_idVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, m_vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_numIndices, m_indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		if (m_normals)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, m_normals, GL_STATIC_DRAW);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
			glEnableVertexAttribArray(1);
		}

		if (m_texCoords)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_idTexCoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 2, m_texCoords, GL_STATIC_DRAW);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
			glEnableVertexAttribArray(2);
		}

		if (m_colors)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_idColors);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, m_colors, GL_STATIC_DRAW);

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
	RELEASE_ARRAY(m_indices);
	RELEASE_ARRAY(m_vertices);
	RELEASE_ARRAY(m_normals);
	RELEASE_ARRAY(m_texCoords);
	RELEASE_ARRAY(m_colors);

	m_numIndices = 0;
	m_numVertices = 0;

	FreeVRam();
}

void R_Geometry::FreeVRam()
{
	if (m_idIndices > 0) { glDeleteBuffers(1, &m_idIndices); m_idIndices = 0; }
	if (m_idVertices > 0) { glDeleteBuffers(1, &m_idVertices); m_idVertices = 0; }
	if (m_idNormals > 0) { glDeleteBuffers(1, &m_idNormals); m_idNormals = 0; }
	if (m_idTexCoords > 0) { glDeleteBuffers(1, &m_idTexCoords); m_idTexCoords = 0; }
	if (m_idColors > 0) { glDeleteBuffers(1, &m_idColors); m_idColors = 0; }

	if (m_idContainer > 0) { glDeleteVertexArrays(1, &m_idContainer); m_idContainer = 0; }
}
