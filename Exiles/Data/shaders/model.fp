#version 400

in vec2 fTexCoord;
in vec3 fNormal;
in vec3 fTangent;
in vec3 fWorldPos;
in vec4 fShadowCoord;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform sampler2DShadow depthTex;
uniform sampler2D ssaoTex;
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

#define M_PI 3.1415926535897932384626433832795

layout(location = 0) out vec4 FragColor;

float CalcShadowFactor(vec4 LightSpacePos)
{
    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;

	if (UVCoords.x >= 0.0 &&  UVCoords.x <= 1.0 &&  UVCoords.y >= 0.0 &&  UVCoords.y <= 1.0)
	{
		float xOffset = 1.0/2048;
		float yOffset = 1.0/2048;

		float Factor = 0.0;

		for (int y = -1 ; y <= 1 ; y++) {
			for (int x = -1 ; x <= 1 ; x++) {
				vec2 Offsets = vec2(x * xOffset, y * yOffset);
				vec3 UVC = vec3(UVCoords + Offsets, z - 0.005);
				Factor += texture(depthTex, UVC);
			}
		}

		return (0.2 + (Factor / 11.25));
	} else 
		return 1.0;
}

void main()
{
	vec4 diffuse = texture2D(diffuseTex, fTexCoord);
	if (diffuse.a < 0.7)
		discard;

	vec3 n = normalize(fNormal);
	vec3 specularColor = vec3(0.0,0.0,0.0);

	vec3 tangent = normalize(fTangent);
	tangent = normalize(tangent - dot(tangent, n) * n);
	vec3 bitangent = cross(tangent, n);
	vec3 texNormal = texture(normalTex, fTexCoord).xyz;
	texNormal = 2.0 * texNormal - vec3(1.0, 1.0, 1.0);

	mat3 TBN = mat3(tangent, bitangent, n);
	n = normalize(TBN * texNormal);

	float shadowFracor = CalcShadowFactor(fShadowCoord);

	vec3 l = -normalize(lightDirection);
	vec3 v = -normalize(fWorldPos - cameraPosition);
	vec3 h = normalize(v + l);

	float IOR = 10.42; // Index of Reflection. 2.42 - diamond index
	float F0 = pow((IOR - 1) / (IOR + 1),2);
	float Ap = 256; // power, gloss, ..
	
	float S = clamp((Ap + 2) / (8 * M_PI) * pow(dot(n,h), Ap) * pow(F0 + (1 - F0) * (1 - dot(h,v)), 5),0,1);

	float light = dot(n,l) + S;

	FragColor = vec4(diffuse.rgb * light * shadowFracor, 1.0);
}
