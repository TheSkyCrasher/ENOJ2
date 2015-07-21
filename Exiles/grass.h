#pragma once

#include <string>
#include <vector>
#include "texture.h"
#include "math.h"
#include "shader.h"
#include "camera.h"

class Grass
{
public:
	Grass();

	void Draw(Camera* camera, const Matrix4f& lightMVP);

	virtual ~Grass();
private:
	static const int GRASS_TYPES = 1;
	Texture* m_grassTex;

	GLuint m_VAO;
	GLuint m_VBO[5];
	Shader m_shader;
	std::vector<Matrix4f> m_grassMatrices;
	std::vector<float> m_grassRnd;
};