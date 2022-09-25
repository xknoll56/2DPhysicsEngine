#pragma once

class TestScene : Scene
{
public: 
	CircleCollider c1;
	CircleCollider c2;
	TestScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		c1.position = { 1,1 };

		c2.position = { 1, -5 };
		world.circleColliders.push_back(&c1);
		world.circleColliders.push_back(&c2);
		c1.velocity = { 0,-1 };
		c2.velocity = { 0,1 };
		c1.angularVelocity = 1.0f;

		c1.addForce({ 0, -80000 });
		c2.addForce({ 0, 20000 });
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		//renderer->camPos = c1.position;
		world.stepNoSpaceDivision(dt);
		renderer->DrawCircleCollider(&c1, { 1,1,1,1 });
		renderer->DrawCircleCollider(&c2, { 1,1,1,1 });
	}
};