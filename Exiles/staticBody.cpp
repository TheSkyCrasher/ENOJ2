#include "staticBody.h"

StaticBody::StaticBody(Mesh* mesh, btCollisionShape* collisionShape, Vector3f pos) : MeshObject(mesh), m_collisionShape(collisionShape)
{
	SetPos(pos);
	m_motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.GetX(), pos.GetY(), pos.GetZ())));
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, m_motionState, m_collisionShape, btVector3(0,0,0));

	m_physicsBody = new btRigidBody(rigidBodyCI);
}

StaticBody::~StaticBody()
{
	if (m_motionState) delete m_motionState;
	if (m_collisionShape) delete m_collisionShape;
	if (m_physicsBody) delete m_physicsBody;
}