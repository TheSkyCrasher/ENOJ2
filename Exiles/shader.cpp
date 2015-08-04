#include "shader.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>

std::map<std::string, ShaderData*> Shader::s_resourceMap;

static void CheckShaderError(int shader, int flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (!success)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		fprintf(stderr, "%s: '%s'\n", errorMessage.c_str(), error);
	}
}
std::vector<std::string> Split(const std::string& s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = (unsigned int)s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}
static std::string LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open(("Data/shaders/" + fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			if (line.find("#include") == std::string::npos)
				output.append(line + "\n");
			else
			{
				std::string includeFileName = Split(line, ' ')[1];
				includeFileName = includeFileName.substr(1, includeFileName.length() - 2);

				std::string toAppend = LoadShader(includeFileName);
				output.append(toAppend + "\n");
			}
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
};

ShaderData::ShaderData(const std::string& fileName, bool enableGeometric)
{
	m_program = glCreateProgram();

	if (m_program == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vertexShaderText = LoadShader(fileName + ".vp");
	std::string fragmentShaderText = LoadShader(fileName + ".fp");
	std::string geometricShaderText;

	AddVertexShader(vertexShaderText);
	AddFragmentShader(fragmentShaderText);

	AddAllAttributes(vertexShaderText);

	if (enableGeometric)
	{
		geometricShaderText = LoadShader(fileName + ".gp");
		AddGeometryShader(geometricShaderText);
	}

	CompileShader();

	AddUniforms(vertexShaderText);
	AddUniforms(fragmentShaderText);

	if (enableGeometric)
		AddUniforms(geometricShaderText);
}

ShaderData::~ShaderData()
{
	for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		glDetachShader(m_program, *it);
		glDeleteShader(*it);
	}
	glDeleteProgram(m_program);
}

void ShaderData::AddVertexShader(const std::string& text)
{
	AddProgram(text, GL_VERTEX_SHADER);
}

void ShaderData::AddGeometryShader(const std::string& text)
{
	AddProgram(text, GL_GEOMETRY_SHADER);
}

void ShaderData::AddFragmentShader(const std::string& text)
{
	AddProgram(text, GL_FRAGMENT_SHADER);
}

void ShaderData::AddProgram(const std::string& text, int type)
{
	int shader = glCreateShader(type);

	if (shader == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n", type);
	}

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar InfoLog[1024];

		glGetShaderInfoLog(shader, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shader, InfoLog);
	}

	glAttachShader(m_program, shader);
	m_shaders.push_back(shader);
}

void ShaderData::AddAllAttributes(const std::string& vertexShaderText)
{
	static const std::string ATTRIBUTE_KEY = "layout";

	size_t attributeLocation = vertexShaderText.find(ATTRIBUTE_KEY);
	while (attributeLocation != std::string::npos)
	{
		bool isCommented = false;
		size_t lastLineEnd = vertexShaderText.rfind(";", attributeLocation);

		if (lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = vertexShaderText.substr(lastLineEnd, attributeLocation - lastLineEnd);
			isCommented = potentialCommentSection.find("//") != std::string::npos;
		}

		if (!isCommented)
		{
			size_t begin = attributeLocation + ATTRIBUTE_KEY.length();
			size_t end = vertexShaderText.find(";", begin);

			std::string attributeLine = vertexShaderText.substr(begin + 1, end - begin - 1);

			std::regex reg("\\( *location *= *([\\d]) *\\) *[a-z\\d]{1,} *[a-z\\d]{1,} *(\\w{1,})", std::regex::optimize);
			std::smatch match;

			if (std::regex_search(attributeLine, match, reg)) {
				const std::string location = match[1];
				const std::string name = match[2];

				glBindAttribLocation(m_program, atoi(location.c_str()), name.c_str());
			}
		}
		attributeLocation = vertexShaderText.find(ATTRIBUTE_KEY, attributeLocation + ATTRIBUTE_KEY.length());
	}
}

void ShaderData::AddUniforms(const std::string& shaderText)
{
	static const std::string UNIFORM_KEY = "uniform";

	size_t uniformLocation = shaderText.find(UNIFORM_KEY);
	while (uniformLocation != std::string::npos)
	{
		bool isCommented = false;
		size_t lastLineEnd = shaderText.rfind(";", uniformLocation);

		if (lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = shaderText.substr(lastLineEnd, uniformLocation - lastLineEnd);
			isCommented = potentialCommentSection.find("//") != std::string::npos;
		}

		if (!isCommented)
		{
			size_t begin = uniformLocation + UNIFORM_KEY.length();
			size_t end = shaderText.find(";", begin);

			std::string uniformLine = shaderText.substr(begin + 1, end - begin - 1);

			begin = uniformLine.find(" ");
			std::string uniformName = uniformLine.substr(begin + 1);
			std::string uniformType = uniformLine.substr(0, begin);

			if (uniformType == "int")
				m_uniformsi[uniformName] = glGetUniformLocation(m_program, uniformName.c_str());
			else if (uniformType == "float")
				m_uniformsf[uniformName] = glGetUniformLocation(m_program, uniformName.c_str());
			else if (uniformType == "vec3")
				m_uniformsVector3f[uniformName] = glGetUniformLocation(m_program, uniformName.c_str());
			else if (uniformType == "mat4")
				m_uniformsMatrix4f[uniformName] = glGetUniformLocation(m_program, uniformName.c_str());
			else if (uniformType == "sampler2D" || uniformType == "sampler2DShadow")
				m_uniformsi[uniformName] = glGetUniformLocation(m_program, uniformName.c_str());
		}
		uniformLocation = shaderText.find(UNIFORM_KEY, uniformLocation + UNIFORM_KEY.length());
	}
}

void ShaderData::CompileShader()
{
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Invalid shader program");
}

Shader::Shader(const std::string& fileName, bool enableGeometric) : m_fileName(fileName)
{
	std::map<std::string, ShaderData*>::const_iterator it = s_resourceMap.find(fileName);
	if (it != s_resourceMap.end())
	{
		m_shaderData = it->second;
		m_shaderData->AddReference();
	}
	else
	{
		m_shaderData = new ShaderData(fileName, enableGeometric);
		s_resourceMap.insert(std::pair<std::string, ShaderData*>(fileName, m_shaderData));
	}
}

Shader::~Shader()
{
	if (m_shaderData && m_shaderData->RemoveReference())
	{
		if (m_fileName.length() > 0)
			s_resourceMap.erase(m_fileName);

		delete m_shaderData;
	}
}

void Shader::Bind()
{
	glUseProgram(m_shaderData->m_program);
}

void Shader::SetUniform(const std::string& name, int value)
{
	glUniform1i(m_shaderData->m_uniformsi[name], value);
}

void Shader::SetUniform(const std::string& name, float value)
{
	glUniform1f(m_shaderData->m_uniformsf[name], value);
}

void Shader::SetUniform(const std::string& name, const Vector3f& value)
{
	glUniform3f(m_shaderData->m_uniformsVector3f[name], value.GetX(), value.GetY(), value.GetZ());
}

void Shader::SetUniform(const std::string& name, const Matrix4f& value)
{
	glUniformMatrix4fv(m_shaderData->m_uniformsMatrix4f[name], 1, GL_FALSE, &value[0][0]);
}