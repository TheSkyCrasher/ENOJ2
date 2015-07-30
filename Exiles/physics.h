#pragma once

#include "rigidBody.h"
#include "staticBody.h"
#include <vector>

class Physics
{
public:
	static void Init(const float g);
	static void Delete();
	static void Update(const float dt);

	static void AddBody(MeshObject* body);
private:
	static btBroadphaseInterface* m_broadphase; // collision algorithm
	static btDefaultCollisionConfiguration* m_collisionConfiguration;
	static btCollisionDispatcher* m_collisionDispatcher;
	static btSequentialImpulseConstraintSolver* m_solver;

	static btDiscreteDynamicsWorld* m_world;

	static std::vector<MeshObject*> m_objects;
	static unsigned int m_osize; // size of m_objects (maybe it will save RAM)
};

