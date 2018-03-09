// Include .h files
#include "PhysicsEngineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"

// Other includes
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <glm\glm.hpp>

// Typedefs

//============================================================================================================================================
// Constructors

// Constructor
PhysicsEngineApp::PhysicsEngineApp()
{
}

// Deconstructor
PhysicsEngineApp::~PhysicsEngineApp()
{
}

//============================================================================================================================================
// Startup and Shutdown

// Startup
bool PhysicsEngineApp::startup()
{
	// Increase the 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 64435U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, 0));
	m_physicsScene->setTimeStep(0.01f);

	// Create new object
	collSphere1 = new Sphere(glm::vec2(-30, 20), glm::vec2(25, -35),  glm::vec2(0, 0), 2.0f, 8, 1.0f, glm::vec4(1, 1, 0, 1)); // Pink
	collSphere2 = new Sphere(glm::vec2(30, -20), glm::vec2(-20, -38), glm::vec2(0, 0), 4.0f, 5, 1.0f, glm::vec4(1, 0, 1, 1)); // Yellow
	collSphere3 = new Sphere(glm::vec2(0, 0),    glm::vec2(0, -45),   glm::vec2(0, 0), 4.0f, 2, 1.0f, glm::vec4(1, 0, 0, 1)); // Red
	collPlane1  = new Plane (glm::vec2(0, -1), -40.0f, glm::vec4(1, 0, 1, 1)); // Upper Plane
	collPlane2  = new Plane (glm::vec2(0, 1),  -40.0f, glm::vec4(1, 0, 1, 1)); // Bottom Plane
	collPlane3  = new Plane (glm::vec2(-1, 0), -40.0f, glm::vec4(1, 0, 1, 1)); // Left Plane
	collPlane4  = new Plane (glm::vec2(1, 0),  -40.0f, glm::vec4(1, 0, 1, 1)); // Right Plane
	collAABB1   = new AABB  (glm::vec2(-30, -20),  glm::vec2(20, 0),  glm::vec2(0, 0), glm::vec2(5, 10), 2.0f, 1, glm::vec4(1, 1, 1, 1)); // White
	collAABB2   = new AABB  (glm::vec2(30, 20),    glm::vec2(-20, 0), glm::vec2(0, 0), glm::vec2(8, 12), 2.0f, 1, glm::vec4(0, 1, 1, 1));

	// Add said object(actor) into the physics scene
	m_physicsScene->addActor(collSphere1);
	m_physicsScene->addActor(collSphere2);
	m_physicsScene->addActor(collSphere3);
	m_physicsScene->addActor(collPlane1);
	m_physicsScene->addActor(collPlane2);
	m_physicsScene->addActor(collPlane3);
	m_physicsScene->addActor(collPlane4);
	m_physicsScene->addActor(collAABB1);
	m_physicsScene->addActor(collAABB2);

	//setupContinuousDemo(glm::vec2(-100, -50), 3.14 * 0.33, 25, -10);

	return true;
}

// Shutdown
void PhysicsEngineApp::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
	delete collSphere1;
	delete collSphere2;
	delete collSphere3;
	delete collPlane1;
	delete collPlane2;
	delete collAABB1;
	delete collAABB2;
}

//============================================================================================================================================
// Setup Kinematic Formula Demo

// Setup Kinematic Formula Demo
void PhysicsEngineApp::setupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;
	float radius = 1.0f;
	int segments = 12;
	glm::vec4 colour = glm::vec4(1, 1, 0, 1);

	glm::vec2 velocity = glm::vec2(sin(inclination), cos(inclination)) * speed;

	float x;
	float y;

	while (t <= 5)
	{
		x = velocity.x * t;
		y = velocity.y * t + (0.5 * gravity * t * t);
		aie::Gizmos::add2DCircle(glm::vec2(x, y), radius, segments, colour);
		t += tStep;
	}
}

//============================================================================================================================================
// Update Function

// Update
void PhysicsEngineApp::update(float deltaTime)
{
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	// Call physics scene functions
	m_physicsScene->update(deltaTime);										// Update physics scene
	m_physicsScene->updateGizmos();											// Update gizmos

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

//============================================================================================================================================
// Draw Function

// Draw
void PhysicsEngineApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	//
	float aspectRatio = (float)getWindowWidth() / (float)getWindowHeight();
	aie::Gizmos::draw2D(glm::ortho<float>(-100.0f, 100.0f, 
										  -100.0f / aspectRatio, 100.0f / aspectRatio,
										  -1.0f, 1.0f));

	// done drawing sprites
	m_2dRenderer->end();
}