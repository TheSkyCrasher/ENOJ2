#pragma once

#include <GL\glew.h>
#include <string>
#include "shader.h"
#include "camera.h"
#include "mesh.h"

class CubemapTexture
{
public:
	CubemapTexture(const std::string& filename);
	virtual ~CubemapTexture();

	bool Load();

	void Bind(unsigned int unit);

private:
	GLuint m_texture;
};

class SkyBox
{
public:
	SkyBox();
	virtual ~SkyBox();

	void Draw(Camera* camera);
private:
	Shader m_shader;
	Model* m_cube;
	CubemapTexture* m_texture;
};