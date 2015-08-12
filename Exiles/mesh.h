#pragma once

#include "vertex.h"
#include "texture.h"
#include "util.h"
#include "timer.h"
#include "shader.h"
#include "animator.h"

#include <SDL2\SDL.h>
#include <GL/glew.h>
#include <string>
#include <vector>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

#define ANIMATION_TICKS_PER_SECOND 1.0
#define BONES_PER_MESH 60

class Mesh
{
public:
	Mesh(const std::string& name);
	virtual ~Mesh();

	//void SetCollisionShape(btCollisionShape* collisionShape) { m_collisionShape = collisionShape; }

	//void SetComplexCollisionShape() { m_collisionShape = GetConvexHullShape(); }

	btConvexHullShape* GetConvexHullShape();

	unsigned int* GetVBO() { return &m_vbo; }
	unsigned int* GetIBO() { return &m_ibo; }
	inline const std::string& GetName() { return m_name; }

	// TODO: make it private
	unsigned int numIndices;
	unsigned int materialIndex;
	std::vector<Vector3f> vertices;
private:
	unsigned int m_vbo;
	unsigned int m_ibo;
	std::string  m_name;

	//btCollisionShape* m_collisionShape;
};

class ModelUnit : public ReferenceCounter
{
public:
	ModelUnit();
	virtual ~ModelUnit();

	//TODO: make it private
	std::vector<Mesh*> m_meshes;
	std::vector<Texture*> m_textures;
	btCollisionShape* m_collisionShape;
	const aiScene* m_scene;
	Animator* m_animator;
};

class Model
{
public:
	Model(const std::string& fileName);
	Model(Vertex* vertices, int vertSize, int* indices, int indexSize);
	virtual ~Model();

	void Draw(Shader* shader);
	void DrawMesh(unsigned int id = 0) const;

	Mesh* GetMesh(unsigned int id) { return m_meshData->m_meshes[id]; }
	Mesh* GetMesh(const std::string& name) { for each (auto mesh in m_meshData->m_meshes) if (mesh->GetName() == name) return mesh; }

	void ToggleAnimationPlaying() { m_meshData->m_animator->TogglePlaying(); }

	void SetCollisionShape(btCollisionShape* collisionShape) { m_meshData->m_collisionShape = collisionShape; }
	btCollisionShape* GetCollisionShape() { return m_meshData->m_collisionShape; }

	void SetAnimation(unsigned int id) { m_meshData->m_animator->SetAnimationIndex(id); }
	void SetShaderLocation(unsigned int shaderLocation) { m_shaderLocation = shaderLocation; }

	unsigned int GetNumMeshes() { return m_meshData->m_meshes.size(); }
private:
	unsigned int m_shaderLocation;
	unsigned int m_matrixLocation;

	static std::map<std::string, ModelUnit*> s_resourceMap;

	void DrawNode(aiNode* m_node);
	void InitMesh(Vertex* vertices, int vertSize, int* indices, int indexSize, unsigned int materialIndex, const std::string& name);

	ModelUnit* m_meshData;
	std::string m_fileName;
};