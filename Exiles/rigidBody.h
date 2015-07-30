#pragma once

#include "meshObject.h"

class RigidBody : public MeshObject
{
public:
	RigidBody(Mesh* mesh, btCollisionShape* collisionShape, btScalar m_mass, Vector3f pos = Vector3f());
	virtual ~RigidBody();

	virtual btRigidBody* GetPhysicsBody() { return m_physicsBody; }

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
	btCollisionShape* m_collisionShape;
	btScalar m_mass;
	btVector3 m_fallInertia;
	btRigidBody* m_physicsBody;
};