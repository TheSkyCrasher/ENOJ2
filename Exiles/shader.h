#pragma once
#include <string>
#include <GL/glew.h>
#include <map>
#include "transform.h"
#include "camera.h"
#include "texture.h"

class Shader
{
public:
	Shader(const std::string& fileName, bool instance = false);

	void Bind();
	void Update(const std::string& name, int value);
	void Update(const std::string& name, float value);
	void Update(const std::string& name, const Vector3f& value);
	void Update(const std::string& name, const Matrix4f& value);

	template<class T>
	void AddUniform(const std::string& name);

	inline GLuint& GetShaderID() { return m_program; }

	virtual ~Shader();
protected:
private:
	const static int NUM_UNIFORMS = 5;
	void operator=(const Shader& shader) {}
	Shader(const Shader& shader) {}

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

	enum
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,

		NUM_SHADERS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];

	std::map<std::string, GLuint> m_uniformsi;
	std::map<std::string, GLuint> m_uniformsf;
	std::map<std::string, GLuint> m_uniformsVector3f;
	std::map<std::string, GLuint> m_uniformsMatrix4f;
	std::map<std::string, GLuint> m_uniformsTexture;
};