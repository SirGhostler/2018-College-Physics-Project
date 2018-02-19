#pragma once
// Include .h files
#include "PhysicsObject.h"

// Other includes
#include <glm\vec2.hpp>
#include <glm\glm.hpp>

// Typedefs

class Rigidbody : public PhysicsObject
{

public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, float rotation, float mass, float elasticity);
	//~Rigidbody();
		
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();

	//============================================================================================================================================
	// Apply Force

	void applyForce(glm::vec2 force);
	void applyForceToActor(Rigidbody* actor2, glm::vec2 force);

	//============================================================================================================================================
	// Collision

	virtual bool checkCollision(PhysicsObject* pOther) = 0;
	void resolveCollision(Rigidbody* actor2);

	//============================================================================================================================================
	// Getters and Setters

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getRotation()		{ return m_rotation; }
	float getMass()			{ return m_mass; }
	float getElasticity()	{ return m_elasticity; }

	void setPosition(glm::vec2 position);
	void setVelocity(glm::vec2 velocity);

protected:
	//============================================================================================================================================
	// Position, Velocity, Acceleration

	glm::vec2 m_position;
	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;

	//============================================================================================================================================
	// Mass, Rotation, Elasticity

	float m_mass;
	float m_rotation;
	float m_elasticity;

	//============================================================================================================================================
	// Linear and Angular Drag

	float m_linearDrag;
	float m_minLinearDrag;
	float m_angularDrag;
	float m_minAngularDrag;

	float m_angularVelocity;
};