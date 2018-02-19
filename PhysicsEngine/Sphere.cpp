#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, float mass, float radius, float elasticity, glm::vec4 color) : Rigidbody(SPHERE, position, velocity, acceleration, 0, mass, elasticity)
{
	m_radius = radius;
	m_color = color;
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_color);
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	Sphere* otherSphere = dynamic_cast<Sphere*>(pOther);
	if (otherSphere != nullptr)
	{
		float distance = glm::distance(getPosition(), otherSphere->getPosition());

		if (distance < (getRadius() + otherSphere->getRadius()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return 0;
	}
}