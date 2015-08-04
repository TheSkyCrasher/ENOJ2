#pragma once
#include <string>
#include <GL/glew.h>
#include <map>
#include "util.h"
#include "math.h"

class ShaderData : public ReferenceCounter
{
public:
	ShaderData(const std::string& fileName, bool enableGeometric);
	virtual ~ShaderData();

	int m_program;
	inline std::vector<int>& GetShaders() { return m_shaders; }

	std::map<std::string, GLuint> m_uniformsi;
	std::map<std::string, GLuint> m_uniformsf;
	std::map<std::string, GLuint> m_uniformsVector3f;
	std::map<std::string, GLuint> m_uniformsMatrix4f;
private:
	void AddVertexShader(const std::string& text);
	void AddGeometryShader(const std::string& text);
	void AddFragmentShader(const std::string& text);
	void AddProgram(const std::string& text, int type);

	void AddAllAttributes(const std::string& vertexShaderText);
	void AddUniforms(const std::string& shaderText);
	void CompileShader();

	std::vector<int> m_shaders;
};

class Shader
{
public:
	Shader(const std::string& fileName, bool enableGeometric = false);

	void Bind();
	void SetUniform(const std::string& name, int value);
	void SetUniform(const std::string& name, float value);
	void SetUniform(const std::string& name, const Vector3f& value);
	void SetUniform(const std::string& name, const Matrix4f& value);

	int GetProgram() const { return m_shaderData->m_program; }

	virtual ~Shader();
protected:
private:
	static std::map<std::string, ShaderData*> s_resourceMap;

	ShaderData* m_shaderData;
	std::string m_fileName;
};