#pragma once
// Include .h files
#include "PhysicsObject.h"
#include "RigidBody.h"

// Other includes
#include <glm\glm.hpp>

// Typedefs

class Plane : public PhysicsObject
{

public:

	//============================================================================================================================================
	// Constructors

	Plane();
	Plane(const glm::vec2& normal, float distanceToOrigin, glm::vec4 color);
	//~Plane();

	//============================================================================================================================================
	// Getters And Setters

	glm::vec2 getNormal() { return m_normal; }
	float getDistanceToOrigin() { return m_distanceToOrigin; }

	//============================================================================================================================================
	// Misc

	virtual void fixedUpdate(glm::vec2 gravity, float dt) override {}
	virtual void debug() override {}
	virtual void makeGizmo();
	void resolveCollision(Rigidbody* actor2, glm::vec2 cnor);

private:
	glm::vec2 m_normal;
	float m_distanceToOrigin;

};