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

	ball = new Sphere(glm::vec2(-100, 0), glm::vec2(0, 0), 2.0f, 5, glm::vec4(1, 1, 0, 1));
	m_physicsScene->addActor(ball);
	ball2 = new Sphere(glm::vec2(100, 0), glm::vec2(0, 0), 4.0f, 5, glm::vec4(1, 1, 1, 1));
	m_physicsScene->addActor(ball2);
	ball3 = new Sphere(glm::vec2(0, 0), glm::vec2(0, 0), 0.1f, 5, glm::vec4(1, 0, 1, 1));
	m_physicsScene->addActor(ball3);

	return true;
}

void PhysicsEngineApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
	delete ball;
	delete ball2;
	delete ball3;
}

void PhysicsEngineApp::update(float deltaTime) {
	aie::Gizmos::clear();


	ball->applyForce(glm::vec2(10, 0));

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// input example
	aie::Input* input = aie::Input::getInstance();

	// create colors
	static const glm::vec4 colors[] = {
		glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1),
		glm::vec4(0, 0, 1, 1), glm::vec4(0.8f, 0, 0.5f, 1),
		glm::vec4(0, 1, 1, 1)
	};

	static const int rows = 5;
	static const int columns = 10;
	static const int hSpace = 1;
	static const int vSpace = 1;

	static const glm::vec2 scrExtents(100, 50);
	static const glm::vec2 boxExtents(7, 3);
	static const glm::vec2 startPos(
		-(columns >> 1)*((boxExtents.x * 2) + vSpace) + boxExtents.x + (vSpace / 2.0f),
		 scrExtents.y - ((boxExtents.y * 2) + hSpace));

	// draw the grids
	glm::vec2 pos;
	for (int y = 0; y < rows; y++) {
		pos = glm::vec2(startPos.x, startPos.y - (y * ((boxExtents.y * 2) + hSpace)));
		for (int x = 0; x < columns; x++) {
			aie::Gizmos::add2DAABBFilled(pos, boxExtents, colors[y]);
			pos.x += (boxExtents.x * 2) + vSpace;
		}
	}

	//// draw the ball
	//aie::Gizmos::add2DCircle(glm::vec2(0, -35), 3, 12, glm::vec4(1,0,0,1));

	// paddle movement
	if (input->isKeyDown(aie::INPUT_KEY_LEFT)) {
		paddlePos.x += -50.0f * deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT)) {
		paddlePos.x += 50.0f * deltaTime;
	}

	// draw the player's paddle
	aie::Gizmos::add2DAABBFilled(glm::vec2(paddlePos.x, -40), glm::vec2(12, 2),
		glm::vec4(1, 0, 1, 1));

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