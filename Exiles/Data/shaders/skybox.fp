#version 400

in vec3 fTexCoord;

uniform samplerCube skybox;

void main()
{
    gl_FragColor = texture(skybox, fTexCoord);
}