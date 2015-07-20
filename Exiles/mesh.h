#pragma once

#include "math.h"
#include "texture.h"
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <string>
#include <vector>

struct Vertex
{
public:
	Vertex(float x, float y, float z, float u, float v, float nx = 0.0f, float ny = 0.0f , float nz = 0.0f)
	{
		this->pos = Vector3f(x, y, z);
		this->texCoord = Vector2f(u, v);
		this->normal = Vector3f(nx, ny, nz);
	}

	Vector3f pos;
	Vector2f texCoord;
	Vector3f normal;
	Vector3f tangent;
};

class MeshObject
{
public:
	MeshObject(unsigned int indexSize, unsigned int materialIndex);
	virtual ~MeshObject();

	inline unsigned int GetVBO() { return m_vbo; }
	inline unsigned int GetIBO() { return m_ibo; }
	inline int GetSize() { return m_size; }
protected:
private:
	MeshObject(MeshObject& other) {}
	void operator=(MeshObject& other) {}

	unsigned int m_vbo;
	unsigned int m_ibo;
	unsigned int m_size;
	unsigned int m_materialIndex;
};

class Mesh
{
public:
	Mesh(const std::string& fileName);
	Mesh(Vertex* vertices, unsigned int vertSize, unsigned int* indices, unsigned int indexSize);
	virtual ~Mesh();

	void Draw() const;
protected:
private:
	Mesh(Mesh& mesh) {}
	void operator=(Mesh& mesh) {}

	void InitMesh(Vertex* vertices, unsigned int vertSize, unsigned int* indices, unsigned int indexSize, unsigned int materialIndex = 0);

	void CalcNormals(Vertex* vertices, unsigned int vertSize, unsigned int* indices, unsigned int indexSize);
	void CalcTangents(Vertex* vertices, unsigned int vertSize, unsigned int* indices, unsigned int indexSize);

	std::string m_fileName;
	std::vector<MeshObject*> m_meshObjects;
	std::vector<Texture*> m_textures;
};