#version 430

in Vertex_Out {
	vec3 position;
	vec3 color;
	vec2 tex;
	vec3 normal;
} _in;

layout(location = 0) out vec3 position;
layout(location = 1) out vec3 color;
layout(location = 2) out vec3 tex;
layout(location = 3) out vec3 normal;

uniform sampler2D textureMapping; 

void main() 
{ 
    position = _in.position; 
    color = texture(textureMapping, _in.tex).xyz; 
    normal = normalize(_in.normal); 
    tex = vec3(_in.tex, 0.0); 
}