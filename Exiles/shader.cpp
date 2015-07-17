#include "shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string& fileName)
{
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader("Data/shaders/" + fileName + ".vp"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader("Data/shaders/" + fileName + ".fp"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(m_program, m_shaders[i]);

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");
	glBindAttribLocation(m_program, 3, "tangent");
	glBindAttribLocation(m_program, 4, "bitangent");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");
}

template<>
void Shader::AddUniform<int>(const std::string& name)
{
	m_uniformsi[name] = glGetUniformLocation(m_program, name.c_str());
}

template<>
void Shader::AddUniform<float>(const std::string& name)
{
	m_uniformsf[name] = glGetUniformLocation(m_program, name.c_str());
}

template<>
void Shader::AddUniform<Vector3f>(const std::string& name)
{
	m_uniformsVector3f[name] = glGetUniformLocation(m_program, name.c_str());
}

template<>
void Shader::AddUniform<Matrix4f>(const std::string& name)
{
	m_uniformsMatrix4f[name] = glGetUniformLocation(m_program, name.c_str());
}

Shader::~Shader()
{
	for(unsigned int i = 0; i < NUM_SHADERS; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }

	glDeleteProgram(m_program);
}

void Shader::Bind()
{
	glUseProgram(m_program);
}

void Shader::Update(const std::string& name, int value)
{
	glUniform1i(m_uniformsi[name], value);
}

void Shader::Update(const std::string& name, float value)
{
	glUniform1f(m_uniformsf[name], value);
}

void Shader::Update(const std::string& name, const Vector3f& value)
{
	glUniform3f(m_uniformsVector3f[name], value.GetX(), value.GetY(), value.GetZ());
}

void Shader::Update(const std::string& name, const Matrix4f& value)
{
	glUniformMatrix4fv(m_uniformsMatrix4f[name], 1, GL_FALSE, &value[0][0]);
}

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
    GLuint shader = glCreateShader(type);

    if(shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

    const GLchar* p[1];
    p[0] = text.c_str();
    GLint lengths[1];
    lengths[0] = text.length();

    glShaderSource(shader, 1, p, lengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

    return shader;
}
