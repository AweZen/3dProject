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

vec3 inWorld(vec3 pos)
{
	 return vec3(world * vec4(pos, 1));
}

void main()
{
	mat4 pvw = proj * view * world;
	gl_Position   = pvw * vec4(position , 1);
	_out.position = inWorld(position);
	_out.color    = color;
    _out.tex      = tex;
	_out.normal   = normalize(normal);
} 
