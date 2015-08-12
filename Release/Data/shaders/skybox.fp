#version 400

in vec3 fTexCoord;

uniform samplerCube skybox;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(skybox, fTexCoord);
}