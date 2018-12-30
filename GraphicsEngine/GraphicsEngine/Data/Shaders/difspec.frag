#version 330 core
out vec4 FragColor;
//layout (location = 0) out vec3 gPosition;
//layout (location = 1) out vec3 gNormal;
//layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuse;
uniform sampler2D specular;

void main()
{
	FragColor = vec4(texture(specular, TexCoords).rgb, 1.0);
	//gPosition = FragPos;
	//gNormal = normalize(Normal);
	//gAlbedoSpec.rgb = texture(diffuse, TexCoords).rgb;
	//gAlbedoSpec.a = texture(specular, TexCoords).r;
}