// Include .h files
#include "Plane.h"
#include "PhysicsEngineApp.h"
#include "RigidBody.h"

// Other includes
#include <Gizmos.h>

// Typedefs

//============================================================================================================================================
// Constructors

// Constructor
Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	// Set the distance from the plane's origin
	m_distanceToOrigin = 0;
	// Set the plane's normal
	m_normal = glm::vec2(0, 1);
}

Plane::Plane(const glm::vec2 & normal, float distanceToOrigin, glm::vec4 color)
	: PhysicsObject(ShapeType::PLANE)
	, m_normal(normal)
	, m_distanceToOrigin(distanceToOrigin)
{
}


//============================================================================================================================================
// Gizmo Functions

// Make Gizmo
void Plane::makeGizmo()
{
	// Set the plane's length
	float lineSegmentLength = 300;
	// Set the center point
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	
	// easy to rotate normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, colour);
}

//============================================================================================================================================
// Collision Functions

// Resolve Collision
void Plane::resolveCollision(Rigidbody* actor2, glm::vec2 cnor)
{
	// Set the normal
	glm::vec2 normal = m_normal;
	// Get the relative velocity from the second actor
	glm::vec2 relativeVelocity = actor2->getVelocity();

	// Set elasticity
	float elasticity = 1;
	// Set the impulse magnitude
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * ((1 / actor2->getMass())));

	// Set force
	glm::vec2 force = normal * j;

	// Apply velocity to the second actor
	actor2->setVelocity(actor2->getVelocity() + force / actor2->getMass());
}