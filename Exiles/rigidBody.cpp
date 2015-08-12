#include "rigidBody.h"

void RigidBody::Init()
{
	m_motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(GetPos().GetX(), GetPos().GetY(), GetPos().GetZ())));
	GetModel()->GetCollisionShape()->calculateLocalInertia(m_mass, m_fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass, m_motionState, GetModel()->GetCollisionShape(), m_fallInertia);

	m_physicsBody = new btRigidBody(rigidBodyCI);
}

RigidBody::~RigidBody()
{
	if (m_motionState) delete m_motionState;
	if (m_physicsBody) delete m_physicsBody;
}