#version 330 core

in vec3 vertColor;
in vec2 uv;
in vec3 normal;

uniform sampler2D textureColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(texture(textureColor, uv).rgb, 1.0);
}