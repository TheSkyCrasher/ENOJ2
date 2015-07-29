#include "grass.h"
#include "timer.h"
#include "perlinNoise.h"
#include "window.h"
#include <iostream>

Grass::Grass() : m_shader("grass")
{
	m_grassTex = new Texture("images/grass0.png", true);

	PerlinNoise pn(rand() % 100000);

	for (unsigned int i = 0; i < 100; ++i)
	{
		for (unsigned int j = 0; j < 100; ++j)
		{
			m_grassRnd.push_back(pn.noise((float)i / 100.0f * 2, (float)j / 100.0f * 2, 0.8f));
			m_grassMatrices.push_back(Matrix4f().InitTranslation(Vector3f(5.0f - (float)i / 10.0f, 0.0f, 5.0f - (float)j / 10.0f) * -1.0f) * Matrix4f().InitRotationEuler((1.5f - ToRadians(rand() % 180 + 1)* *m_grassRnd.end())*0.1f, ToRadians(rand() % 360 + 1), (1.5f - ToRadians(rand() % 180 + 1)* *m_grassRnd.end())*0.1f));
		}
	}

	Vector3f positions[] = {
		Vector3f(-0.25f, 0.4f, 0.0f),
		Vector3f(-0.25f, 0.9f, 0.0f),
		Vector3f(0.25f, 0.9f, 0.0f),
		Vector3f(0.25f, 0.4f, 0.0f),
	};

	Vector2f texcoords[] = {
		Vector2f(0.0f, 1.0f),
		Vector2f(0.0f, 0.0f),
		Vector2f(1.0f, 0.0f),
		Vector2f(1.0f, 1.0f),
	};

	int indices[] = {
		0,1,2,
		0,2,3
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(5, m_VBO);

	GLsizei vsize = sizeof(Vector4f);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f)* 4, &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f)* 4, &texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, m_grassMatrices.size() * sizeof(Matrix4f), &m_grassMatrices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vsize, (GLvoid*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vsize, (GLvoid*)(vsize));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vsize, (GLvoid*)(2 * vsize));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vsize, (GLvoid*)(3 * vsize));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_grassMatrices.size(), &m_grassRnd[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(float), (GLvoid*)0);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)* 6, &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	m_shader.Bind();
	m_shader.SetUniform("depthTex", 0);
	m_shader.SetUniform("diffuseTex", 1);
	glUseProgram(0);
}

static float time = 0.0f;

void Grass::Draw(Camera* camera, const Matrix4f& lightMVP)
{
	m_shader.Bind();
	m_shader.SetUniform("MVP", camera->GetViewProjection());
	m_shader.SetUniform("lightMVP", lightMVP);
	m_shader.SetUniform("time", time);
	time+= Timer::deltaTime();

	m_grassTex->Bind(1);

	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(m_VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_grassMatrices.size());
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

Grass::~Grass()
{
	glDeleteBuffers(5, m_VBO);
	glDeleteVertexArrays(1, &m_VAO);

	if (m_grassTex) delete m_grassTex;

	std::cout << "GRASS DELETED\n";
}