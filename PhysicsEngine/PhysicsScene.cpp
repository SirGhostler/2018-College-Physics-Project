// Include .h files
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"

// Other includes
#include <iostream>
#include <algorithm>
#include <cassert>
#include <list>

// Typedefs
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

//============================================================================================================================================
// Constructors

// Constructor
PhysicsScene::PhysicsScene()
{
	// Set time step to 0.0f and gravity to 0, 0.0f
	m_timeStep = 0.0f;
	m_gravity = glm::vec2(0, 0.0f);
}

// Deconstructor
PhysicsScene::~PhysicsScene()
{
	// Delete all actors in the scene
	for (auto& actor : m_actors)
	{
		delete actor;
	}
}

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
	// Return false, two static objects won't really collide anyway.
	return false;
}

// Plane to Sphere Collision
bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Run Sphere to Plane collission function in reverse.
	return sphere2Plane(obj2, obj1);
}

// Sphere to Plane Collision
bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	Plane  *plane = dynamic_cast <Plane*> (obj2);

	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistanceToOrigin();

		// if we are behind plane then we flip the normal
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1; sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;

		if (intersection > 0)
		{
			//set sphere velocity to zero here
			return true;
		}
	} return false;
}

// Sphere to Sphere Collision
bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Cast Sphere 1 to Obj1 and Sphere 2 to Obj2
	Sphere *sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere *sphere2 = dynamic_cast<Sphere*>(obj2);

	// Check if both Spheres exist
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		// Get the radii of both Spheres and combine them
		float combinedRadii = (sphere1->getRadius() + sphere2->getRadius());
		// Get the distance between both Spheres
		float objectDistance = glm::distance(sphere1->getPosition(), sphere2->getPosition());

		// Check if the distance between both Spheres is less than their combined radii
		if (objectDistance < combinedRadii)
		{
			// Call resolve collision function
			sphere1->resolveCollision(sphere2);
		}
	}

	// Return false if either Sphere doesn't exist
	return false;
}

//============================================================================================================================================
// Actor Functions

// Add Actor
void PhysicsScene::addActor(PhysicsObject* actor)
{
	// Push the new Actor onto the m_actors stack
	m_actors.push_back(actor);
}

// Remove Actor
void PhysicsScene::removeActor(PhysicsObject* actor)
{
	// Remove specified actor from the stack
	std::remove(std::begin(m_actors), std::end(m_actors), actor);
}

//============================================================================================================================================
// Update Functions

// Update
void PhysicsScene::update(float dt) {
	static std::list<PhysicsObject*> dirty; 
	
	// Update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	// Increment the accumulated time by delta time
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
				Plane* pPlane = dynamic_cast<Plane*>(pActor);

				// Check to make sure pRigid exists (is not nullptr) before checking for collision
				if (pRigid && pRigid->checkCollision(pOther) == true)
				{
					pRigid->applyForceToActor(dynamic_cast<Rigidbody*>(pOther), pRigid->getVelocity() * pRigid->getMass());
					dirty.push_back(pRigid); dirty.push_back(pOther);
				}

				/*if (pPlane && pRigid->checkCollision(pOther) == true)
				{
					pPlane->resolveCollision(pRigid);
					dirty.push_back(pPlane); dirty.push_back(pRigid); dirty.push_back(pOther);
				}*/
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
	// Set count to 0 for iteration
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}