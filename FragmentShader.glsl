#version 430

in Vertex_Out {
	vec3 position;
	vec3 color;
	vec2 tex;
	vec3 normal;
} _in;

out vec4 fragmentColor;

uniform sampler2D myTex;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec4 ambientLight;
uniform vec3 pos;
//uniform vec3 colourFromImGui;

void main () {
	//Diffuse
	vec3 lightVector = normalize(lightPosition - _in.position);
	float brightness = dot(lightVector,_in.normal);
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0f);

	//Specular
	vec3 reflectVec = reflect(-lightVector, _in.normal);
	vec3 lightToPosDirVec = normalize(pos - _in.position);
	float s = clamp(dot(reflectVec, lightToPosDirVec), 0, 1);
	s = pow(s, 30);
	vec4 specular = vec4(s, 0, s, 0);


//	vec3 lightToPosDirVec = normalize(lightPosition -_in.position);
//	vec3 reflectVec = normalize(reflect(lightToPosDirVec,normalize(_in.normal)));
//	vec3 posToViewDir = normalize(_in.position - pos);
//	float specular = pow(max(dot(posToViewDir,reflectVec),0),2); 

	//fragmentColor = texture(myTex, _in.tex) * vec4(_in.color * lightColor , 0);	

	//fragmentColor = texture(myTex, _in.tex)  * max(vec4(_in.color,1 )* diffuseLight + vec4( _in.color,1) * ambientLight,vec4(0));
	
	fragmentColor = max(diffuseLight * vec4(_in.color * lightColor,1)* texture(myTex, _in.tex) + specular * vec4(_in.color * lightColor,1) *  texture(myTex, _in.tex),0);//clamp(diffuseLight + specular , 0.05f, 1)  * texture(myTex, _in.tex)  * vec4(_in.color * lightColor , 0);
}