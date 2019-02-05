#version 430


vec2 CalcTexCoord()
{
   return gl_FragCoord.xy / gScreenSize;
}

void main()
{
   	vec2 TexCoord = CalcTexCoord();
   	vec3 WorldPos = texture(gPositionMap, TexCoord).xyz;
   	vec3 Color = texture(gColorMap, TexCoord).xyz;
   	vec3 Normal = texture(gNormalMap, TexCoord).xyz;
   	Normal = normalize(Normal);

   	FragColor = vec4(Color, 1.0) * CalcDirectionalLight(WorldPos, Normal);
}