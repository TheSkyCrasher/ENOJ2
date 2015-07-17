#version 400

in vec2 fTexCoord;
in vec3 fNormal;
in vec3 fTangent;
in vec3 fWorldPos;
in vec4 fShadowCoord;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform sampler2D depthTex;
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

float CalcShadowFactor(vec4 LightSpacePos)
{
    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
    float Depth = texture(depthTex, UVCoords).x;
    if (Depth < (z + 0.00001 * tan(acos(clamp(dot(fNormal,-lightDirection),0.0,1.0)))))
        return 0.2;
    else
        return 1.0;
}

void main()
{
	vec3 normal = normalize(fNormal);
	vec3 tangent = normalize(fTangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	vec3 texNormal = texture(normalTex, fTexCoord).xyz;
	texNormal = 2.0 * texNormal - vec3(1.0, 1.0, 1.0);

	mat3 TBN = mat3(tangent, bitangent, normal);
	vec3 tNormal = normalize(TBN * texNormal);

	float diffuseFactor = dot(tNormal, -normalize(lightDirection));

	vec4 specularColor;
	float specularIntensity = texture2D(specularTex, fTexCoord).r;

	if (diffuseFactor > 0) {
        vec3 vertexToEye = normalize(cameraPosition - fWorldPos);
        vec3 lightReflect = normalize(reflect(lightDirection, tNormal));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, 64);
            specularColor = vec4(vec3(1.0,1.0,1.0) * specularIntensity * specularFactor, 1.0f);
        }
    }

	float shadowFracor = CalcShadowFactor(fShadowCoord);
	if (shadowFracor < 1.0)
	{
		specularColor = vec4(0.0,0.0,0.0,1.0);
	}

	gl_FragColor = (texture2D(diffuseTex, fTexCoord) * (clamp(dot(-normalize(lightDirection), tNormal), 0.0, 1.0) + specularColor)) * shadowFracor;
}
