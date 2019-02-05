#version 330 

layout (location = 0) in vec3 position; 

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

void main()
{ 
   	gl_Position = proj * view * world * vec4(position, 1.0);
} 