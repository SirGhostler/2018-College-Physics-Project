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
	// Plane collides with Plane	// Plane collides with Sphere	// Plane collides with AABB
	PhysicsScene::plane2Plane,		PhysicsScene::plane2Sphere,		PhysicsScene::plane2AABB,
	// Sphere collides with Plane	// Sphere collides with Sphere	// Sphere collides with AABB
	PhysicsScene::sphere2Plane,		PhysicsScene::sphere2Sphere,	PhysicsScene::sphere2AABB,
	// AABB collides with Plane		// AABB collides with Sphere	// AABB collides with AABB
	PhysicsScene::AABB2Plane,		PhysicsScene::sphere2AABB,		PhysicsScene::AABB2AABB,
};

// Collision Check
void PhysicsScene::checkForCollision()
{
	// Create actor count
	int actorCount = m_actors.size();
	// Check for collisions against all objects except this one
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
				// Check if a collision occured
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

bool PhysicsScene::plane2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

// Sphere to Plane Collision
bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Cast the Sphere to Obj1 and the Plane to Obj2
	Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	Plane  *plane = dynamic_cast <Plane*> (obj2);

	// Check if both objects actually exist
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistanceToOrigin();

		// If the sphere is behind the plane, we flip the normal
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1; sphereToPlane *= -1;
		}

		// Get the intersection from the sphere to the plane
		float intersection = sphere->getRadius() - sphereToPlane;

		// Check if they're actually intersecting
		if (intersection > 0)
		{
			// Call resolve collision function
			separateCollision(sphere, plane, plane->getNormal(), (sphere->getRadius() - intersection));
			plane->resolveCollision(sphere);
		}
	}
	// Return false if either object doesn't exist
	return false;
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
			separateCollision(sphere1, sphere2, glm::normalize(sphere1->getPosition() - sphere2->getPosition()), (combinedRadii - objectDistance));
			sphere1->resolveCollision(sphere2);
		}
	}

	// Return false if either Sphere doesn't exist
	return false;
}

bool PhysicsScene::sphere2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::AABB2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::AABB2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::AABB2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

// Separate Collsion
void PhysicsScene::separateCollision(PhysicsObject* obj1, PhysicsObject* obj2, glm::vec2 normal, float overlap)
{
	// Assign IsStatic result of each object to variables
	bool firstIsStatic = obj1->isStatic();
	bool secondIsStatic = obj2->isStatic();

	// If the first Object is static
	if (firstIsStatic)
	{
		// Cast rigidbody to Object 1
		Rigidbody *rigidBody = dynamic_cast<Rigidbody*>(obj1);
		if (rigidBody)
		{
			// Set a variable with the current position
			glm::vec2 currentPosition = rigidBody->getPosition();
			// Set position of the Object
			rigidBody->setPosition(currentPosition + (overlap * normal));
		}
	}
	// If the second Object is static
	if (secondIsStatic)
	{
		// Cast rigidbody to Object 2
		Rigidbody *rigidBody = dynamic_cast<Rigidbody*>(obj2);
		if (rigidBody)
		{
			// Set a variable with the current position
			glm::vec2 currentPosition = rigidBody->getPosition();
			// Set position of the Object
			rigidBody->setPosition(currentPosition - (overlap * normal));
		}
	}
	// If both Objects are static
	if (!firstIsStatic && !secondIsStatic)
	{
		// Cast rigidbody1 to Object 1
		Rigidbody *rigidBody1 = dynamic_cast<Rigidbody*>(obj1);
		// Cast rigidbody2 to Object 2
		Rigidbody *rigidBody2 = dynamic_cast<Rigidbody*>(obj2);
		if (rigidBody1 && rigidBody2)
		{
			// Set a variable with Object 1's and Object 2's current positions
			glm::vec2 currentPosition1 = rigidBody1->getPosition();
			glm::vec2 currentPosition2 = rigidBody2->getPosition();
			// Set position of the Objects
			rigidBody1->setPosition(currentPosition1 + ((overlap * normal) * 0.5f));
			rigidBody2->setPosition(currentPosition2 - ((overlap * normal) * 0.5f));
		}
	}
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
	
	// Check if accumulated time is equal to or greater than the timestep
	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		// Subtract accumulated time from timestep
		accumulatedTime -= m_timeStep; 
		
		// Run collision check function
		checkForCollision();
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