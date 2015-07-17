#version 400

in vec2 fTexCoord;
in vec3 fNormal;
in vec3 fTangent;
in vec3 fWorldPos;

uniform sampler2D samplers[4];
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

void main()
{
	vec3 normal = normalize(fNormal);
	vec3 tangent = normalize(fTangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	vec3 texNormal = texture(samplers[1], fTexCoord).xyz;
	texNormal = 2.0 * texNormal - vec3(1.0, 1.0, 1.0);

	mat3 TBN = mat3(tangent, bitangent, normal);
	vec3 tNormal = normalize(TBN * texNormal);

	float diffuseFactor = dot(tNormal, -normalize(lightDirection));

	vec4 specularColor;
	float specularIntensity = texture2D(samplers[2], fTexCoord).r;

	if (diffuseFactor > 0) {
        vec3 vertexToEye = normalize(cameraPosition - fWorldPos);
        vec3 lightReflect = normalize(reflect(lightDirection, tNormal));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, 32);
            specularColor = vec4(vec3(1.0,1.0,1.0) * specularIntensity * specularFactor, 1.0f);
        }
    }

	gl_FragColor = texture2D(samplers[0], fTexCoord) * (clamp(dot(-normalize(lightDirection), tNormal), 0.0, 1.0) + specularColor);
}
