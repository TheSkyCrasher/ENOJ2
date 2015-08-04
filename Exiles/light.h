#pragma once

#include <GL\glew.h>
#include "shader.h"
#include "math.h"
#include "window.h"

class DirectionalLight
{
public:
	DirectionalLight();

	void SetRender();
	void BindTexture(unsigned int unit);

	static const int DEPTH_TEXTURE_WIDTH = 2048;
	static const int DEPTH_TEXTURE_HEIGHT = 2048;

	inline const Matrix4f& GetMVP() const { return m_MVP; }
	inline const Vector3f& GetDirection() const { return m_direction; }
	inline Shader* GetShader() { return &m_shader; }

	virtual ~DirectionalLight();
private:
	Shader m_shader;

	Matrix4f m_MVP;

	Vector3f m_direction;

	GLuint m_frameBuffer;
	GLuint m_depthTexture;
	static GLuint CreateDepthTexture(unsigned int width, unsigned int height);
};