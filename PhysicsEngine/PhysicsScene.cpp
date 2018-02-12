// Include .h files.
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Sphere.h"

// Other includes.
#include <iostream>
#include <algorithm>
#include <cassert>
#include <list>

// Typedefs.
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

//============================================================================================================================================
// Collision Functions

// Collision Function Array
static fn collisionFunctionArray[] = 
{
	// Plane collides with Plane	// Plane collides with Sphere
	PhysicsScene::plane2Plane,		PhysicsScene::plane2Sphere,
	// Sphere collides with Plane	// Sphere collides with Sphere
	PhysicsScene::sphere2Plane,		PhysicsScene::sphere2Sphere,
};

// Collision Check
void PhysicsScene::checkForCollision()
{
	int actorCount = m_actors.size();
	//need to check for collisions against all objects except this one.
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			// using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

// Plane to Plane Collision
bool PhysicsScene::plane2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

// Plane to Sphere Collision
bool PhysicsScene::plane2Sphere(PhysicsObject *, PhysicsObject *)
{
	return false;
}

// Sphere to Plane Collision
bool PhysicsScene::sphere2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

// Sphere to Sphere Collision
bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//try to cast objects to sphere and sphere
	Sphere *sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere *sphere2 = dynamic_cast<Sphere*>(obj2);
	//if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float combinedRadii = (sphere1->getRadius() + sphere2->getRadius());
		float objectDistance = glm::distance(sphere1->getPosition(), sphere2->getPosition());

		if (objectDistance < combinedRadii)
		{
			sphere1->setVelocity(glm::vec2(0,0));
			sphere2->setVelocity(glm::vec2(0, 0));
		}
	}
	return false;
}

//============================================================================================================================================
// Constructors

// Constructor
PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.0f;
	m_gravity = glm::vec2(0, 0.0f);
}

// Deconstructor
PhysicsScene::~PhysicsScene()
{
	for (auto& actor : m_actors)
	{
		delete actor;
	}
}

//============================================================================================================================================
// Actor Functions

// Add Actor
void PhysicsScene::addActor(PhysicsObject * actor)
{
	// Push the new Actor onto the m_actors stack.
	m_actors.push_back(actor);
}

// Remove Actor
void PhysicsScene::removeActor(PhysicsObject * actor)
{
	std::remove(std::begin(m_actors), std::end(m_actors), actor);
}

//============================================================================================================================================
// Update Functions

// Update
void PhysicsScene::update(float dt) {
	static std::list<PhysicsObject*> dirty; 
	
	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;
	
	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep; 
		
		// check for collisions (ideally you'd want to have some sort of // scene management in place)
		
		for (auto pActor : m_actors)
		{
			for (auto pOther : m_actors)
			{
				if (pActor == pOther)
					continue;

				if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() && std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
					continue;

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);

				if (pRigid->checkCollision(pOther) == true)
				{
					pRigid->applyForceToActor(dynamic_cast<Rigidbody*>(pOther), pRigid->getVelocity() * pRigid->getMass());
					dirty.push_back(pRigid); dirty.push_back(pOther);
				}
			}
		}
		dirty.clear();
	}
}

// Update Gizmos
void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

// Debugging
void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}