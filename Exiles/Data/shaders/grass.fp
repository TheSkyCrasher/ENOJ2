#version 400

uniform sampler2D diffuseTex;
uniform sampler2D depthTex;

in vec2 fTexCoord;
in vec4 fShadowCoord;
in float fDistance;

layout(location = 0) out vec4 FragColor;

float CalcShadowFactor(vec4 LightSpacePos)
{
    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
    float Depth = texture(depthTex, UVCoords).x;
    if (Depth < (z + 0.0001 ))
        return 0.2;
    else
        return 1.0;
}

void main()
{
	vec4 diffuse = texture2D(diffuseTex, fTexCoord);

	diffuse.a = clamp(diffuse.a * ( 1.0 / ( 1 + fDistance) + 1.0), 0.0, 1.0);

	float shadow = CalcShadowFactor(fShadowCoord);

	if (diffuse.a < 0.5)
		discard;

	FragColor = vec4(diffuse.rgb * shadow,diffuse.a);
}