#include "RigidBody.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) : PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	//m_acceleration = glm::vec2(0.0f, 0.0f);
	m_rotation = rotation;
	m_mass = mass;
	m_shapeID = shapeID;
	m_acceleration = glm::vec2(0, 0);
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	// F = m * a
	// a = F / m
	// v += a * t

	applyForce(gravity * m_mass);
	m_velocity += m_acceleration * timeStep;
	m_position += m_velocity * timeStep;

	m_acceleration = glm::vec2(0, 0);
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 force)
{
	glm::vec2 acc = force / m_mass;
	m_acceleration += acc;
}

void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
{
	applyForce(force);
	actor2->applyForce(-force);
}

void Rigidbody::setVelocity(glm::vec2 velocity)
{
	m_velocity = velocity;
}
