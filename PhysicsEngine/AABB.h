#pragma once
// Include .h files
#include "RigidBody.h"

// Other includes
#include <glm\glm.hpp>

// Typedefs

class AABB : public Rigidbody
{

public:

	//============================================================================================================================================
	// Constructors

	AABB(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, glm::vec2 extents, float mass, float radius, glm::vec4 color);
	//~AABB();

	//============================================================================================================================================
	// Getters And Setters

	glm::vec2 getExtents() { return m_extents; } // Get Extents
	glm::vec4 getColor() { return m_color; } // Get Color

	//============================================================================================================================================
	// Misc

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);
	float m_minX;
	float m_maxX;
	float m_minY;
	float m_maxY;

protected:
	glm::vec2 m_extents;
	glm::vec4 m_color;
};

