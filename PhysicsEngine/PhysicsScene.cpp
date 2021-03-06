// Include .h files
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"

// Other includes
#include <iostream>
#include <algorithm>
#include <cassert>
#include <list>
#include <glm/glm.hpp>

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
	PhysicsScene::AABB2Plane,		PhysicsScene::AABB2Sphere,		PhysicsScene::AABB2AABB,
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
	// Return false, two static objects won't really collide anyway
	return false;
}

// Plane to Sphere Collision
bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Run Sphere to Plane collission function in reverse
	return sphere2Plane(obj2, obj1);
}

// Plane to AABB Collision
bool PhysicsScene::plane2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Run AABB to Plane collission function in reverse
	return AABB2Plane(obj2, obj1);
}

// Sphere to Plane Collision
bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Cast the Sphere to Obj1 and the Plane to Obj2
	Sphere *sphere = dynamic_cast <Sphere*> (obj1);
	Plane  *plane  = dynamic_cast <Plane*>  (obj2);

	// Check if both objects actually exist
	if (sphere != nullptr && plane != nullptr)
	{
		// Get the Plane's normal
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
			separateCollision(sphere, plane, -collisionNormal, (sphere->getRadius() - intersection));
			plane->resolveCollision(sphere, collisionNormal);
			return true;
		}
	}
	// Return false if either object doesn't exist
	return false;
}

// Sphere to Sphere Collision
bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Cast Sphere 1 to Obj1 and Sphere 2 to Obj2
	Sphere *sphere1 = dynamic_cast <Sphere*> (obj1);
	Sphere *sphere2 = dynamic_cast <Sphere*> (obj2);

	// Check if both Spheres exist
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		// Get the radii of both Spheres and combine them
		float combinedRadii = (sphere1->getRadius() + sphere2->getRadius());
		// Get the distance between both Spheres
		float objectDistance = glm::distance(sphere1->getPosition(), sphere2->getPosition());

		// A variable for the collision normal
		glm::vec2 collisionNormal(glm::normalize(sphere1->getPosition() - sphere2->getPosition()));

		// Check if the distance between both Spheres is less than their combined radii
		if (objectDistance < combinedRadii)
		{
			// Call resolve collision function
			separateCollision(sphere1, sphere2, -collisionNormal, (combinedRadii - objectDistance));
			sphere1->resolveCollision(sphere2, collisionNormal);
			return true;
		}
	}

	// Return false if either Sphere doesn't exist
	return false;
}

// Sphere to AABB Collision
bool PhysicsScene::sphere2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Run AABB to Sphere collission function in reverse
	return AABB2Sphere(obj2, obj1);
}

// AABB to Plane Collision
bool PhysicsScene::AABB2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Cast the AABB to Obj1 and the Plane to Obj2
	AABB  *aabb  = dynamic_cast <AABB*>  (obj1);
	Plane *plane = dynamic_cast <Plane*> (obj2);

	// Check if both objects actually exist
	if (aabb != nullptr && plane != nullptr)
	{
		// Get the Plane's normal
		glm::vec2 collisionNormal = plane->getNormal();

		// Store the corners of the AABB into variables
		glm::vec2 aabbTopLeftCorner  = aabb->m_min + glm::vec2(0, aabb->getExtents().y * 2);
		glm::vec2 aabbTopRightCorner = aabb->m_max;
		glm::vec2 aabbBotLeftCorner  = aabb->m_min;
		glm::vec2 aabbBotRightCorner = aabb->m_min + glm::vec2(aabb->getExtents().x * 2, 0);

		// Dot product the corners by the Plane's normal
		float o1 = (glm::dot(aabbTopLeftCorner,  collisionNormal) - plane->getDistanceToOrigin());
		float o2 = (glm::dot(aabbTopRightCorner, collisionNormal) - plane->getDistanceToOrigin());
		float o3 = (glm::dot(aabbBotLeftCorner,  collisionNormal) - plane->getDistanceToOrigin());
		float o4 = (glm::dot(aabbBotRightCorner, collisionNormal) - plane->getDistanceToOrigin());

		// Variable for the lowest (furthest) overlap, set it to o1 by default
		float lowestValue = o1;

		// Checks to find the lowest overlap
		if (!lowestValue)
		{
			// Checks to find which of the aformentioned variables is the lower (if there is one)
			// Check if o2 is the lowest
			if (o2 < lowestValue) { lowestValue = o2; }
			// Check if o3 is the lowest
			if (o3 < lowestValue) { lowestValue = o3; }
			// Check if o4 is the lowest
			if (o4 < lowestValue) { lowestValue = o4; }
		}

		// Check if any of the corners are below the plane
		if ((o1 < 0) || (o2 < 0) || (o3 < 0) || (o4 < 0))
		{
			// Call resolve collision function
			separateCollision(aabb, plane, -collisionNormal, lowestValue);
			plane->resolveCollision(aabb, collisionNormal);
			return true;
		}
	}
	// Return false if either object doesn't exist
	return false;
}

// AABB to Sphere Collision
bool PhysicsScene::AABB2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// Cast the AABB to Obj1 and the Sphere to Obj2
	AABB   *aabb   = dynamic_cast <AABB*>   (obj1);
	Sphere *sphere = dynamic_cast <Sphere*> (obj2);

	// Check if both objects exist
	if (aabb != nullptr && sphere != nullptr)
	{
		//float clampX = glm::clamp(sphere->getPosition().x, aabb->m_minX, aabb->m_maxX);
		//float clampY = glm::clamp(sphere->getPosition().y, aabb->m_minY, aabb->m_maxY);

		// Get a clamped position with the AABB and the Sphere
		glm::vec2 clampedPosition = glm::clamp(sphere->getPosition(), aabb->m_min, aabb->m_max);

		// A variable for the distance between the Sphere and the clamped position
		float objectDistance = glm::length(sphere->getPosition() - clampedPosition);

		// A variable for the collision normal
		glm::vec2 collisionNormal = glm::normalize(sphere->getPosition() - clampedPosition);

		// A variable for the overlap
		float overlap = (sphere->getRadius() - objectDistance);

		// Check if the objectDistance is less than the Sphere's radius
		if (objectDistance < sphere->getRadius())
		{
			// Call resolve collision function
			separateCollision(aabb, sphere, collisionNormal, overlap);
			aabb->resolveCollision(sphere, collisionNormal);
			return true;
		}
	}

	// Return false if either object doesn't exist
	return false;
}

// AABB to AABB Collision
bool PhysicsScene::AABB2AABB(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//// Cast AABB 1 to Obj1 and AABB 2 to Obj2
	AABB *aabb1 = dynamic_cast <AABB*> (obj1);
	AABB *aabb2 = dynamic_cast <AABB*> (obj2);

	// Check if both AABBs actually exist
	if (aabb1 != nullptr && aabb2 != nullptr)
	{
		// Check if the AABBs are colliding
		if ((aabb1->m_max.x) >= (aabb2->m_min.x) && (aabb1->m_min.x) <= (aabb2->m_max.x) &&
			(aabb1->m_min.y) <= (aabb2->m_max.y) && (aabb1->m_max.y) >= (aabb2->m_min.y))
		{
			// Get absolute values stored in variables to be used to obtain the collision normal
			float a1 = (aabb1->m_max.x) - (aabb2->m_min.x); // Right
			float a4 = (aabb1->m_max.y) - (aabb2->m_min.y); // Top
			float a2 = (aabb2->m_max.x) - (aabb1->m_min.x); // Left
			float a3 = (aabb2->m_max.y) - (aabb1->m_min.y); // Bottom

			// Create the actual variable for the collision normal
			glm::vec2 collisionNormal = glm::vec2(1, 0);

			// Create a variable for the overlap, set it to the right by default
			float overlap = a1;

			// Checks to find which of the aformentioned variables is the lower (if there is one)
			// Check if a2 is the lowest
			if (std::abs(a2) < std::abs(overlap)) { collisionNormal = glm::vec2(-1, 0); overlap = a2; }
			// Check if a3 is the lowest
			if (std::abs(a3) < std::abs(overlap)) { collisionNormal = glm::vec2(0, -1); overlap = a3; }
			// Check if a4 is the lowest
			if (std::abs(a4) < std::abs(overlap)) { collisionNormal = glm::vec2(0, 1);  overlap = a4; }

			// Call resolve collision function
			separateCollision(aabb1, aabb2, collisionNormal, overlap);
			aabb1->resolveCollision(aabb2, collisionNormal);
		}
	}

	// Return false if either object doesn't exist
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
			rigidBody1->setPosition(currentPosition1 - ((overlap * normal) * 0.5f));
			rigidBody2->setPosition(currentPosition2 + ((overlap * normal) * 0.5f));
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