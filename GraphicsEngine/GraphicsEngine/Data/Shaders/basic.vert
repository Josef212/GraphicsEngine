#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vertColor;
out vec2 uv;
out vec3 normal;

void main()
{
	vertColor = aColor;
	uv.x = aTexCoord.x;
	uv.y = aTexCoord.y * -1.0;
	normal = aNormal;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}