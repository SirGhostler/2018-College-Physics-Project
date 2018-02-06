#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) : Rigidbody(SPHERE, position, velocity, 0, mass)
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