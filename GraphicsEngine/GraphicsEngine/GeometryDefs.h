#ifndef __GEOMETRY_DEFS_H__
#define __GEOMETRY_DEFS_H__

//----------------------------------------------------------------
//Triangle

unsigned int triangleVerticesCount = 3;
unsigned int triangleIndicesCount = 3;

float triangleVertices[] = {
	-0.5f,  -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

int triangleIndices[] = {
	0, 1, 2
};

float triangleNormals[] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

float triangleTexCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.5f, 1.0f
};

float triangleColors[] = {
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f
};


//----------------------------------------------------------------

//----------------------------------------------------------------
//Quad

unsigned int quadVerticesCount = 4;
unsigned int quadIndicesCount = 6;

float quadVertices[] = {
	0.5f,  0.5f, 0.0f,  // top right
	0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};

int quadIndices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

float quadNormals[] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

float quadTexCoords[] = {
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

float quadColors[] = {
	1.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};


//----------------------------------------------------------------

//----------------------------------------------------------------
//Cube

unsigned int cubeVerticesCount = 12 * 6;
unsigned int cubeIndicesCount = 6 * 6;

float s = 0.5f;
float cubeVertices[] = {
	//Front
	-s, -s, s,	s, -s, s,	s, s, s,	-s, s, s,
	//Right
	s, s, s,	s, s, -s,	s, -s, -s,	s, -s, s,
	//Back
	-s, -s, -s,		s, -s, -s,	s, s, -s,	-s, s, -s,
	//Left
	-s, -s, -s,		-s, -s, s,	-s, s, s,	-s, s, -s,
	//Top
	s, s, s,	-s, s, s,	-s, s, -s,	 s, s, -s,
	//Bot
	-s, -s, -s,		s, -s, -s,	 s, -s, s,	 -s, -s, s
};

int cubeIndices[] = {
	0,  1,  2,  0,  2,  3,   //front
	4,  5,  6,  4,  6,  7,   //right
	8,  9,  10, 8,  10, 11,  //back
	12, 13, 14, 12, 14, 15,  //left
	16, 17, 18, 16, 18, 19,  //upper
	20, 21, 22, 20, 22, 23	 //bottom
};

float u = 1.0f;
float cubeNormals[] = {
	//Front
	0.0f, 0.0f, u,		0.0f, 0.0f, u,		0.0f, 0.0f, u,		0.0f, 0.0f, u,
	//Right
	u, 0.0f, 0.0f,		u, 0.0f, 0.0f,		u, 0.0f, 0.0f,		u, 0.0f, 0.0f,
	//Back
	0.0f, 0.0f, -u,		0.0f, 0.0f, -u,		0.0f, 0.0f, -u,		0.0f, 0.0f, -u,
	//Left
	-u, 0.0f, 0.0f,		-u, 0.0f, 0.0f,		-u, 0.0f, 0.0f,		-u, 0.0f, 0.0f,
	//Top
	0.0f, u, 0.0f,		0.0f, u, 0.0f,		0.0f, u, 0.0f,		0.0f, u, 0.0f,
	//Bot
	0.0f, -u, 0.0f,		0.0f, -u, 0.0f,		0.0f, -u, 0.0f,		0.0f, -u, 0.0f
};

float o = 0.0f;
float cubeTexCoords[] = {
	//Front
	o, o,		u, o,	u, u,	o, u,
	//Right
	o, u,		u, u,	u, o,	o, o,
	//Back
	u, o,		o, o,	o, u,	u, u,
	//Left
	o, o,		u, o,	u, u,	o, u,
	//Top
	u, o,		o, o,	o, u,	u, u,
	//Bot
	u, u,		o, u,	o, o,	u, o
};

float cubeColors[] = {
	//Front		0
	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
	//Right		6
	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
	//Back		12
	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
	//Left		18
	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f,
	//Top		24
	0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,
	//Bot		30
	1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 1.0f
};

//----------------------------------------------------------------


//----------------------------------------------------------------

#endif // !__GEOMETRY_DEFS_H__