#version 330 core

in vec3 vertColor;
in vec2 uv;
in vec3 normal;

out vec4 FragColor;

void main()
{
	FragColor = vec4(vertColor, 1.0);
}