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
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

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
				vec3 UVC = vec3(UVCoords + Offsets, z + 0.00001);
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
	vec3 tNormal = normalize(fNormal);
	vec3 specularColor = vec3(0.0,0.0,0.0);

	vec3 normal = tNormal;
	vec3 tangent = normalize(fTangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	vec3 texNormal = texture(normalTex, fTexCoord).xyz;
	texNormal = 2.0 * texNormal - vec3(1.0, 1.0, 1.0);

	mat3 TBN = mat3(tangent, bitangent, normal);
	tNormal = normalize(TBN * texNormal);

	float shadowFracor = CalcShadowFactor(fShadowCoord);
	if (shadowFracor == 1.0)
	{
		float diffuseFactor = dot(tNormal, -normalize(lightDirection));
		float specularIntensity = texture2D(specularTex, fTexCoord).r;

		if (diffuseFactor > 0) {
			vec3 vertexToEye = normalize(cameraPosition - fWorldPos);
			vec3 lightReflect = normalize(reflect(lightDirection, tNormal));
			float specularFactor = dot(vertexToEye, lightReflect);
			if (specularFactor > 0) {
				specularFactor = pow(specularFactor, 64);
				specularColor = vec3(1.0,1.0,1.0) * specularIntensity * specularFactor;
			}
		}
	}

	vec3 light = (clamp(dot(-normalize(lightDirection), tNormal), 0.0, 1.0) + specularColor);

	gl_FragColor = vec4(diffuse.rgb * light * shadowFracor, diffuse.a);
}
