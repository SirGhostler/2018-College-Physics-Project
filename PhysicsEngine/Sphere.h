#pragma once
// Include .h files
#include "RigidBody.h"

// Other includes
#include <glm\glm.hpp>

// Typedefs

class Sphere : public Rigidbody 
{

public:

	//============================================================================================================================================
	// Constructors

	Sphere(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, float mass, float radius, float elasticity, glm::vec4 color);
	//~Sphere();

	//============================================================================================================================================
	// Getters And Setters

	float getRadius() { return m_radius; }
	glm::vec4 getColor() { return m_color; }

	//============================================================================================================================================
	// Misc

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

protected:
	float m_radius;
	glm::vec4 m_color;

};