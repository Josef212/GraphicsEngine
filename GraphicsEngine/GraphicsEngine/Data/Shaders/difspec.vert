#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	//uv.x = aTexCoord.x;
	//uv.y = aTexCoord.y * -1.0;

	vec4 worldPos = model * vec4(aPos, 1.0);
	FragPos = worldPos.xyz;
	TexCoords = aTexCoord;
	Normal = transpose(inverse(mat3(model))) * aNormal;

	gl_Position = projection * view * worldPos;
}