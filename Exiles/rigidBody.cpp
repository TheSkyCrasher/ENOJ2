#include "rigidBody.h"

RigidBody::RigidBody(Mesh* mesh, btCollisionShape* collisionShape, btScalar m_mass, Vector3f pos ) : MeshObject(mesh), m_collisionShape(collisionShape)
{
	SetPos(pos);
	m_motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.GetX(), pos.GetY(), pos.GetZ())));
	m_collisionShape->calculateLocalInertia(m_mass, m_fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass, m_motionState, m_collisionShape, m_fallInertia);

	m_physicsBody = new btRigidBody(rigidBodyCI);
}

RigidBody::~RigidBody()
{
	if (m_motionState) delete m_motionState;
	if (m_collisionShape) delete m_collisionShape;
	if (m_physicsBody) delete m_physicsBody;
}