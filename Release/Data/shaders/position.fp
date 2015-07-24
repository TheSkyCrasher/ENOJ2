#version 400

in vec4 fPosition;

layout (location = 0) out vec4 FragColor; 

void main()
{
    FragColor = vec4(fPosition.xyz,1.0);
}