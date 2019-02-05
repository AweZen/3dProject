#version 430

layout(location = 0) in vec3 positionVertex;
layout(location = 1) in vec2 tex;

layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec3 position;

void main()
{
	gl_Position = vec4(position, 1);
	texCoord = tex;

}