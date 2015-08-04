#include "staticBody.h"

void StaticBody::Init()
{
	m_motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(GetPos().GetX(), GetPos().GetY(), GetPos().GetZ())));
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, m_motionState, GetMesh()->m_mw->m_collisionShape, btVector3(0, 0, 0));

	m_physicsBody = new btRigidBody(rigidBodyCI);
}

StaticBody::~StaticBody()
{
	if (m_motionState) delete m_motionState;
	if (m_physicsBody) delete m_physicsBody;
}