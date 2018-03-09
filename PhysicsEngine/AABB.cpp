// Include .h files
#include "AABB.h"

// Other includes
#include <Gizmos.h>

// Typedefs

//============================================================================================================================================
// Constructors

// Constructor
AABB::AABB(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, glm::vec2 extents, float mass, float radius, glm::vec4 color) : Rigidbody(AABB_, position, velocity, acceleration, 0, mass, radius)
{
	m_extents = extents;
	m_color = color;
	//m_minX = (-(getExtents().x));	// Left
	//m_maxX = (getExtents().x);		// Right
	//m_minY = (-(getExtents().y));	// Bottom
	//m_maxY = (getExtents().y);		// Top

	m_min = m_position - m_extents;
	m_max = m_extents + m_position;
}

void AABB::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::fixedUpdate(gravity, timeStep);

	m_min = m_position - m_extents;
	m_max = m_extents + m_position;
}

//============================================================================================================================================
// Gizmo Functions

// Make Gizmo
void AABB::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(getPosition(), m_extents, m_color, nullptr);
}

//============================================================================================================================================
// Collision Functions

// Check Collision
bool AABB::checkCollision(PhysicsObject * pOther)
{
	return false;
}