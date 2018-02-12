#include "PhysicsEngineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

#include "Sphere.h"

#include <Gizmos.h>
#include <glm\ext.hpp>
#include <glm\glm.hpp>

PhysicsEngineApp::PhysicsEngineApp() {

	paddlePos = glm::vec2(0, -40);
}

PhysicsEngineApp::~PhysicsEngineApp() {

}

bool PhysicsEngineApp::startup() {
	// increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 64435U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	//m_physicsScene->setGravity(glm::vec2(0, -10));
	m_physicsScene->setTimeStep(0.01f);

	ball1 = new Sphere(glm::vec2(-80, 0), glm::vec2(10, 0), 2.0f, 5, glm::vec4(1, 1, 0, 1));
	ball2 = new Sphere(glm::vec2(80, 0), glm::vec2(-10, 0), 4.0f, 5, glm::vec4(1, 0, 1, 1));
	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);

	

	//setupContinuousDemo(glm::vec2(-100, -50), 3.14 * 0.33, 25, -10);

	return true;
}

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

void PhysicsEngineApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
	delete ball1;
	delete ball2;
}

void PhysicsEngineApp::update(float deltaTime) {
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->sphere2Sphere(ball1, ball2);

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsEngineApp::draw() {

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