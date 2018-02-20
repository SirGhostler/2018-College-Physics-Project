// Include .h files
#include "AABB.h"

// Other includes
#include <Gizmos.h>

// Typedefs


AABB::AABB(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, glm::vec2 extents, float mass, float radius, glm::vec4 color) : Rigidbody(AABB_, position, velocity, acceleration, 0, mass, radius)
{
	m_extents = extents;
	m_color = color;
	m_minX = (-(getExtents().x));
	m_maxX = (getExtents().x);
	m_minY = (-(getExtents().y));
	m_maxY = (getExtents().y);
}

void AABB::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(getPosition(), m_extents, m_color, nullptr);
}

bool AABB::checkCollision(PhysicsObject * pOther)
{
	return false;
}
