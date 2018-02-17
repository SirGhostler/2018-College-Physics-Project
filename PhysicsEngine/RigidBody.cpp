#include "RigidBody.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity) : PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;

	m_mass = mass;
	m_shapeID = shapeID;

	m_elasticity = elasticity;

	m_acceleration = glm::vec2(0, 0);

	m_linearDrag = 0.3f;
	m_minLinearDrag = 0.1f;
	m_angularDrag = 0.3f;
	m_minAngularDrag = 0.01f;
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	// F = m * a
	// a = F / m
	// v += a * t

	applyForce(gravity * m_mass);
	m_velocity += m_acceleration /** m_linearDrag*/ * timeStep;
	m_position += m_velocity /** m_angularDrag*/ * timeStep;

	m_acceleration = glm::vec2(0, 0);

	if (length(m_velocity) < m_minLinearDrag)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < m_minAngularDrag)
	{
		m_angularVelocity = 0;
	}
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 force)
{
	glm::vec2 acc = force / m_mass;
	m_acceleration += acc;
}

void Rigidbody::applyForceToActor(Rigidbody* actor2, glm::vec2 force)
{
	applyForce(force);
	actor2->applyForce(-force);
}

void Rigidbody::setVelocity(glm::vec2 velocity)
{
	m_velocity = velocity;
}

void Rigidbody::resolveCollision(Rigidbody* actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;
	float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));

	glm::vec2 force = normal * j;
	

	setVelocity(getVelocity() - force / getMass());
	actor2->setVelocity(actor2->getVelocity() + force / actor2->getMass());
}
