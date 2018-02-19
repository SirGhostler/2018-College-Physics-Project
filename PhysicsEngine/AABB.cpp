// Include .h files
#include "AABB.h"

// Other includes

// Typedefs


AABB::AABB(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, float mass, float extents, glm::vec4 color) : Rigidbody(AABB_, position, velocity, acceleration, 0, mass, extents)
{
	m_extents = extents;
	m_color = color;
}

AABB::~AABB()
{
}

void AABB::makeGizmo()
{
}

bool AABB::checkCollision(PhysicsObject * pOther)
{
	return false;
}
