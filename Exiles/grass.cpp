#include "grass.h"
#include "timer.h"
#include "perlinNoise.h"
#include "window.h"

Grass::Grass() : m_shader("grass", true)
{
	for (unsigned int i = 0; i < GRASS_TYPES; ++i)
	{
		m_grassTypes[i] = new Texture("grass" + std::to_string(i+1) + ".png", true);
	}

	PerlinNoise pn(rand() % 100000);

	for (unsigned int i = 0; i < 100; ++i)
	{
		for (unsigned int j = 0; j < 100; ++j)
		{
			m_grassRnd.push_back(pn.noise((float)i / 100.0f * 2, (float)j / 100.0f * 2, 0.8f));
			m_grassMatrices.push_back(Matrix4f().InitTranslation(Vector3f(5.0f - (float)i / 10.0f, 0.0f, 5.0f - (float)j / 10.0f) * -1.0f) * Matrix4f().InitRotationEuler((0.5f - ToRadians(rand() % 360 + 1)* *m_grassRnd.end())*0.1f, ToRadians(rand() % 360 + 1), (0.5f - ToRadians(rand() % 360 + 1)* *m_grassRnd.end())*0.1f));
		}
	}

	Vertex vertices[] = {
		Vertex(-0.25f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		Vertex(-0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex( 0.25f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex( 0.25f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	};

	unsigned int indices[] = {
		0,1,2,
		0,2,3
	};

	m_quadMesh = new Mesh(vertices, 4, indices, 6);
	m_VAO = m_quadMesh->GetVAO();
	GLuint buffer;
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, m_grassMatrices.size() * sizeof(Matrix4f), &m_grassMatrices[0], GL_STATIC_DRAW);

	GLsizei vsize = sizeof(Vector4f);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vsize, (GLvoid*)0);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vsize, (GLvoid*)(vsize));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vsize, (GLvoid*)(2 * vsize));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vsize, (GLvoid*)(3 * vsize));

	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);

	GLuint buffer2;
	glGenBuffers(1, &buffer2);
	glBindBuffer(GL_ARRAY_BUFFER, buffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 10000, &m_grassRnd[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(float), (GLvoid*)0);
	glVertexAttribDivisor(9, 1);

	glBindVertexArray(0);

	m_shader.AddUniform<Matrix4f>("lightMVP");
	m_shader.AddUniform<Matrix4f>("MVP");
	m_shader.AddUniform<float>("time");
}

static float time = 0.0f;

void Grass::BindShader()
{
	m_shader.Bind();
}

void Grass::Draw(Camera* camera, const Matrix4f& lightMVP)
{
	m_shader.Bind();
	m_shader.Update("MVP", camera->GetViewProjection());
	m_shader.Update("lightMVP", lightMVP);
	m_shader.Update("time", time);
	time+= Timer::deltaTime();

	for (unsigned int i = 0; i < GRASS_TYPES; ++i)
	{
		m_grassTypes[i]->Bind(i+1);
	}

	glDisable(GL_CULL_FACE);
	glBindVertexArray(m_VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_grassMatrices.size());
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
}

Grass::~Grass()
{
	if (m_quadMesh) delete m_quadMesh;
	for (unsigned int i = 0; i < GRASS_TYPES; ++i)
	{
		if (m_grassTypes[i]) delete m_grassTypes[i];
	}
}