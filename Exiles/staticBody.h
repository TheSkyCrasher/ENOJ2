#pragma once

#include "meshObject.h"

class StaticBody : public MeshObject
{
public:
	StaticBody(Mesh* mesh) : MeshObject(mesh) {}
	virtual ~StaticBody();

	virtual void Init();
	void SetCollision(btCollisionShape* collisionShape) { GetMesh()->m_mw->m_collisionShape = collisionShape; }

	virtual btRigidBody* GetPhysicsBody() { return m_physicsBody; }
private:
	btDefaultMotionState* m_motionState;
	btRigidBody* m_physicsBody;
};