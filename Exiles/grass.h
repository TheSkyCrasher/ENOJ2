#pragma once

#include <string>
#include <vector>
#include "texture.h"
#include "math.h"
#include "shader.h"
#include "mesh.h"

class Grass
{
public:
	Grass();

	void BindShader();

	void Draw(Camera* camera, const Matrix4f& lightMVP);

	virtual ~Grass();
private:
	static const int GRASS_TYPES = 1;
	Texture* m_grassTypes[GRASS_TYPES];

	Mesh* m_quadMesh;

	GLuint m_VAO;
	Shader m_shader;
	std::vector<Matrix4f> m_grassMatrices;
	std::vector<float> m_grassRnd;
};