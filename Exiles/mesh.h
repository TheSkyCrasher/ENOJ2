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
	virtual ~MeshObject();

	std::vector<Vector3f> positions;
	std::vector<Vector2f> texCoords;
	std::vector<Vector3f> normals;
	std::vector<Vector3f> tangents;
	std::vector<unsigned int> indices;

	unsigned int m_vao;
	unsigned int m_vbo[5];
	unsigned int m_numIndices;
	unsigned int m_materialIndex;
};

class Mesh
{
public:
	Mesh(const std::string& fileName);

	virtual ~Mesh();

	void Draw() const;
protected:
private:
	Mesh(Mesh& mesh) {}
	void operator=(Mesh& mesh) {}

	std::string m_fileName;
	std::vector<MeshObject*> m_meshObjects;
	std::vector<Texture*> m_textures;
};