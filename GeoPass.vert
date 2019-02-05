#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 normal;

out Vertex_Out
{
    vec3 position;
    vec2 TexCoord;
    vec3 Normal;  
}_out;


uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;
                                        
void main()
{       
    gl_Position         = proj * view * world * vec4(position, 1.0);
    _out.TexCoord      = tex;                  
    _out.Normal        = (world * vec4(normal, 0.0)).xyz;   
    _out.position = (world * vec4(position, 1.0)).xyz;   
};

