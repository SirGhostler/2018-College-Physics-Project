#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include <glm\glm.hpp>

class PhysicsEngineApp : public aie::Application {
public:

	PhysicsEngineApp();
	virtual ~PhysicsEngineApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void setupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);

	glm::vec2 paddlePos;

	PhysicsScene* m_physicsScene;

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	class Sphere* ball;
	class Sphere* ball2;
	class Sphere* ball3;
};