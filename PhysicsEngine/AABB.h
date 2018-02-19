#pragma once
// Include .h files
#include "RigidBody.h"

// Other includes

// Typedefs

class AABB : public Rigidbody
{

public:

	AABB(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, float mass, float extents, glm::vec4 color);
	~AABB();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	float getExtents() { return m_extents; }
	glm::vec4 getColor() { return m_color; }

protected:
	float m_extents;
	glm::vec4 m_color;

};

