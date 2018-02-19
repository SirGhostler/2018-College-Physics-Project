#pragma once
// Include .h files

// Other includes
#include <glm\vec2.hpp>
#include <glm\glm.hpp>

// Typedefs

//============================================================================================================================================
// ShapeType ENUM

enum ShapeType
{
	PLANE = 0,
	SPHERE,
	SHAPE_COUNT
};

class PhysicsObject
{

protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	PhysicsObject() {};
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

	ShapeType getShapeID() { return m_shapeID; }

	bool isStatic();

protected:
	ShapeType m_shapeID;
};