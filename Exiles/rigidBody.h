#pragma once

#include "meshObject.h"

class RigidBody : public MeshObject
{
public:
	RigidBody(Model* mesh) : MeshObject(mesh), m_fallInertia(0, 0, 0) {}
	virtual ~RigidBody();

	virtual void Init();
	virtual btRigidBody* GetPhysicsBody() { return m_physicsBody; }

	void SetMass(const float& mass) { m_mass = mass; }

	void* operator new(size_t i)
	{
		return _aligned_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}
private:
	btDefaultMotionState* m_motionState;
	btScalar m_mass;
	btVector3 m_fallInertia;
	btRigidBody* m_physicsBody;
};