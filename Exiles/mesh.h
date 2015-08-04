#pragma once

#include "vertex.h"
#include "texture.h"
#include "util.h"
#include <GL/glew.h>
#include <string>
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

class MeshSubObject
{
public:
	MeshSubObject(unsigned int indexSize);
	virtual ~MeshSubObject();

	std::vector<Vector3f> m_points;

	unsigned int* GetVBO() { return &m_vbo; }
	unsigned int* GetIBO() { return &m_ibo; }
	inline int GetSize() const { return m_size; }

	unsigned int m_materialIndex;
protected:
private:
	unsigned int m_vbo;
	unsigned int m_ibo;
	unsigned int m_size; // num of indices
};

class MeshSubObjectWrapper : public ReferenceCounter
{
public:
	virtual ~MeshSubObjectWrapper();
	std::vector<MeshSubObject*> m_meshObjects;
	std::vector<Texture*> m_textures;
	unsigned int m_size;
	btCollisionShape* m_collisionShape;
};

class Mesh
{
public:
	Mesh(const std::string& fileName, bool def = false);
	Mesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals);

	virtual ~Mesh();
	btConvexHullShape* GetConvexCollisionShape();
	MeshSubObjectWrapper* m_mw;
	void Draw(bool drawTextures = true) const;
protected:
private:
	static std::map<std::string, MeshSubObjectWrapper*> s_resourceMap;

	void InitMesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals, unsigned int materialIndex);

	std::string m_fileName;
};