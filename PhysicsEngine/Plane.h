#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{

public:

	Plane();
	Plane(const glm::vec2& normal, float distanceToOrigin, glm::vec4 color);
	//~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float dt) override {}
	virtual void debug() override {}
	virtual void makeGizmo();

	glm::vec2 getNormal() { return m_normal; }
	float getDistanceToOrigin() { return m_distanceToOrigin; }

private:
	glm::vec2 m_normal;
	float m_distanceToOrigin;

};