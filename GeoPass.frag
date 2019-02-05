#version 430

in Vertex_Out {
	vec3 position;
	vec3 color;
	vec2 tex;
	vec3 normal;
} _in;
	
	

layout (location = 0) out vec3 PositionOut; 
layout (location = 1) out vec3 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 
layout (location = 3) out vec3 TexCoordOut; 


uniform sampler2D gColorMap;                
											
void main () 
{										
	PositionOut = _in.position;					
	NormalOut      = texture(gColorMap, _in.tex).xyz;	
	NormalOut       = normalize(_in.normal);					
	TexCoordOut     = vec3(_in.tex, 0.0);				
};