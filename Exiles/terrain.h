#pragma once

#include <string>
#include <GL\glew.h>
#include <iostream>
#include <vector>
#include "shader.h"
#include "vertex.h"

class Terrain
{
public:
	Terrain(const std::string& fileName = "heightmap1.png");
	virtual ~Terrain();

	float GetHeight(float x, float y) const { return m_heightScale * float(m_heigtmap[(int)x][(int)y]) / 255.0f; }

	void Draw(const Matrix4f& mvp);
private:
	int m_width;
	int m_height;
	int m_indicesNum;
	float m_heightScale;

	unsigned int m_vbo;
	unsigned int m_ibo;
	unsigned char** m_heigtmap;

	Shader m_shader;
};

