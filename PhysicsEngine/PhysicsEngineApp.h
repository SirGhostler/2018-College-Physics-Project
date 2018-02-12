#pragma once
// Include .h files
#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

// Other includes
#include <glm\glm.hpp>

// Typedefs

class PhysicsEngineApp : public aie::Application {
public:

	PhysicsEngineApp();
	virtual ~PhysicsEngineApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	//============================================================================================================================================
	// Physics Scene

	PhysicsScene* m_physicsScene;

	void setupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);

protected:

	//============================================================================================================================================
	// AIE Objects

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	//============================================================================================================================================
	// Collision Objects

	class Sphere* collSphere1;	// 1st Sphere object
	class Sphere* collSphere2;	// 2nd Sphere object
	class Plane* collPlane1;	// 1st Plane object
	class Plane* collPlane2;	// 2nd Plane object
};