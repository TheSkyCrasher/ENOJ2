#pragma once

#include "meshObject.h"

class StaticBody : public MeshObject
{
public:
	StaticBody(Model* mesh) : MeshObject(mesh) {}
	virtual ~StaticBody();

	virtual void Init();

	virtual btRigidBody* GetPhysicsBody() { return m_physicsBody; }
private:
	btDefaultMotionState* m_motionState;
	btRigidBody* m_physicsBody;
};