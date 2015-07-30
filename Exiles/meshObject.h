#pragma once

#include "mesh.h"
#include "transform.h"
#include "shader.h"
#include "camera.h"
#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>

class MeshObject
{
public:
	MeshObject(Mesh* mesh) : m_mesh(mesh) {}
	virtual ~MeshObject() { if (m_mesh) delete m_mesh; std::cout << "mesh deleted\n"; }

	void RenderMesh(Shader* shader);
	void Draw(Shader* shader, Camera* camera);

	virtual btRigidBody* GetPhysicsBody() = 0;
	void SetPos(float x, float y, float z) { m_transform.SetPos(Vector3f(x, y, z)); }
	void SetPos(const Vector3f& pos) { m_transform.SetPos(pos); }

	void SetRot(float x, float y, float z, float w) { m_transform.SetRot(Quaternion(Vector4f(x, y, z, w))); }

	inline Transform& GetTransform() { return m_transform; }
private:
	Mesh* m_mesh;
	Transform m_transform;
};