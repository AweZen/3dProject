#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 normal;

out Vertex_Out {
	vec3 position;
	vec3 color;
	vec2 tex;
	vec3 normal;
} _out;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	mat4 mvp = proj * view * world;
	mat3 normalMatrix = mat3(world);
	normalMatrix = transpose(inverse(normalMatrix));
	gl_Position   = mvp * vec4(position , 1);
	_out.position = vec3(world * vec4(position, 1.0));
	_out.color    = color;
    _out.tex      = tex;
	_out.normal   = normalize(normal);
} 
