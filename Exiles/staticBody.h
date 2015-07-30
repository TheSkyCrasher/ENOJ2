#pragma once

#include "meshObject.h"

class StaticBody : public MeshObject
{
public:
	StaticBody(Mesh* mesh, btCollisionShape* collisionShape, Vector3f pos = Vector3f());
		virtual ~StaticBody();

	virtual btRigidBody* GetPhysicsBody() { return m_physicsBody; }
private:
	btDefaultMotionState* m_motionState;
	btCollisionShape* m_collisionShape;
	btRigidBody* m_physicsBody;
};