#include "physics.h"

btBroadphaseInterface* Physics::m_broadphase = 0;
btDefaultCollisionConfiguration* Physics::m_collisionConfiguration = 0;
btCollisionDispatcher* Physics::m_collisionDispatcher = 0;
btSequentialImpulseConstraintSolver* Physics::m_solver = 0;
btDiscreteDynamicsWorld* Physics::m_world = 0;
std::vector<MeshObject*> Physics::m_objects;
unsigned int Physics::m_osize = 0;

void Physics::Init(const float g)
{
	m_broadphase = new btDbvtBroadphase();

	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	m_solver = new btSequentialImpulseConstraintSolver;

	m_world = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_world->setGravity(btVector3(0, -g, 0));
}

void Physics::Delete()
{
	delete m_world;
	delete m_solver;
	delete m_collisionDispatcher;
	delete m_collisionConfiguration;
	delete m_broadphase;

	std::cout << "= PHYSICS =\n";
}

void Physics::AddBody(MeshObject* body)
{
	body->Init();
	m_world->addRigidBody(body->GetPhysicsBody());
	m_objects.push_back(body);
	m_osize++;
}

void Physics::Update(const float dt)
{
	m_world->stepSimulation(dt, 10);

	for (unsigned int i = 0; i < m_osize; ++i)
	{
		btTransform trans;
		m_objects[i]->GetPhysicsBody()->getMotionState()->getWorldTransform(trans);
		m_objects[i]->SetPos(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		m_objects[i]->SetRot(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ(), trans.getRotation().getW());
	}
}