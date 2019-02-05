#version 440 core

layout(location = 0) in vec2 TexCoord;

layout(location = 0) out vec4 Color;

in Vertex_Out {
	vec3 position;
	vec3 color;
	vec2 tex;
	vec3 normal;
} _in;

struct Light
{
	vec3 position;
	vec3 color;
};

layout (std140, binding = 0) uniform Lights
{
	Light lights[50];
	int nrOfLights;
};

uniform sampler2D Positions;
uniform sampler2D Normals;

uniform vec3 viewPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec4 ambientLight;

 
vec3 diffuseColor(Light light, vec3 position, vec3 normal)
{
	vec3 lightDir = normalize(light.position.xyz - position);
	float factor = max(dot(normal, lightDir), 0);

	return light.color.rgb * factor;
}

vec3 specularColor(Light light,
				   vec3 fragPosition, vec3 fragNormal,
				   vec3 viewPosition, float specularStrength)
{
	vec3 lightDir = normalize(light.position.xyz - fragPosition);
	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	float factor = pow(max(dot(viewDir, reflectDir), 0), 4);

	return light.color.rgb * factor * 1;
}

void main(){

	vec3 position = texture(Positions, TexCoord).xyz;
	vec3 normal = texture(Normals, TexCoord).xyz;
	vec3 lightVector = normalize(lightPosition - _in.position);
	vec3 lightSum = ambientLight.xyz * 0.1;

	vec3 ambientColor = vec3(1, 1, 1);
	float ambientStrength = 0.1f;

		for(int i = 0; i < nrOfLights; i++)
	{
		float dist = length(lights[i].position - position);
		float weakening = (1.0f / (1.0f + 0.045f * dist + 0.0075f * (dist * dist)));

		lightSum += (
			diffuseColor(lights[i], position, normal) * weakening +
			specularColor(lights[i], position, normal, viewPosition, 1) * weakening
		);
	}
	Color = vec4(lightSum, 1);

}