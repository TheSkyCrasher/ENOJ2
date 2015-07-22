#pragma once

#include "math.h"
#include "texture.h"
#include <GL/glew.h>
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
	MeshObject(unsigned int indexSize);
	virtual ~MeshObject();

	unsigned int* GetVBO() { return &m_vbo; }
	unsigned int* GetIBO() { return &m_ibo; }
	inline int GetSize() { return m_size; }

	unsigned int m_materialIndex;
protected:
private:
	MeshObject(MeshObject& other) {}
	void operator=(MeshObject& other) {}

	unsigned int m_vbo;
	unsigned int m_ibo;
	unsigned int m_size;
};

class Mesh
{
public:
	Mesh(const std::string& fileName);
	Mesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals);

	virtual ~Mesh();

	void Draw(bool drawTextures = true) const;
protected:
private:
	Mesh(Mesh& mesh) {}
	void operator=(Mesh& mesh) {}

	void InitMesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals, unsigned int materialIndex);

	std::string m_fileName;
	std::vector<MeshObject*> m_meshObjects;
	std::vector<Texture*> m_textures;
	unsigned int m_size;
};