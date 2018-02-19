// Include .h files
#include "PhysicsObject.h"
#include "RigidBody.h"

// Other includes

// Typedefs

//============================================================================================================================================
// Collision Functions

// Is (the Object) Static
bool PhysicsObject::isStatic()
{
	// Check the Object's ShapeID, very fast but not fastest
	if (getShapeID() == PLANE)
	{
		// If it's a Plane, return true
		return true;
	}
	else
	{
		// Otherwise, return false
		return false;
	}

	// Alternative solution thanks to Matt, slower but safer
	/* Rigidbody* rb = dynamic_cast<Rigidbody*>(this);
	return rb == nullptr; */
}