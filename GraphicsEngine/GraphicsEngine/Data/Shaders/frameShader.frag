#version 330 core

in vec2 uv;

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main()
{
	FragColor = vec4(texture(gPosition, TexCoords).rgb, 1.0);
}