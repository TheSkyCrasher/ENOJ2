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
	MeshObject(Model* mesh) : m_model(mesh) {}
	virtual ~MeshObject() { if (m_model) delete m_model; std::cout << "m_model deleted\n"; }

	void RenderMesh(Shader* shader);
	void Draw(Shader* shader, Camera* camera);

	virtual btRigidBody* GetPhysicsBody() = 0;
	virtual void Init() = 0;
	void SetPos(float x, float y, float z) { m_transform.SetPos(Vector3f(x, y, z)); }
	void SetPos(const Vector3f& pos) { m_transform.SetPos(pos); }
	const Vector3f& GetPos() { return m_transform.GetPos(); }

	Mesh* GetMesh(unsigned int id) { return m_model->GetMesh(id); }
	Model* GetModel() { return m_model; }

	void SetRot(float x, float y, float z, float w) { m_transform.SetRot(Quaternion(Vector4f(x, y, z, w))); }
	void SetScale(float s) { m_transform.SetScale(s); }

	inline Transform& GetTransform() { return m_transform; }
private:
	Model* m_model;
	Transform m_transform;
};