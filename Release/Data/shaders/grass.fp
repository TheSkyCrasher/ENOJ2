#version 400

uniform sampler2D samplers[2];

in vec2 fTexCoord;
in vec4 fShadowCoord;

float CalcShadowFactor(vec4 shadowCoord)
{
    vec3 ProjCoords = shadowCoord.xyz / shadowCoord.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
    float Depth = texture(samplers[0], UVCoords).x;
    if (Depth < (z + 0.00001,0.0,1.0))
        return 0.3;
    else
        return 1.0;
}

void main()
{
	vec4 diffuse = texture2D(samplers[1], fTexCoord);
	if (diffuse.a < 0.5f)
		discard;

	//float shadowFracor = CalcShadowFactor(fShadowCoord);

	gl_FragColor = diffuse;
}