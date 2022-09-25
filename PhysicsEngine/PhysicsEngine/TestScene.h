#pragma once

class TestScene : Scene
{
public: 
	CircleCollider c1;
	CircleCollider c2;
	std::vector<CircleCollider> colliders;

	TestScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		for (int i = 0; i < 20; i++)
		{
			CircleCollider collider;
			collider.setVelocity(vec2{ (float)(rand() % 2), (float)(rand() % 2) });
			collider.position = (vec2{ (float)(rand() % 5), (float)(rand() % 5) });
			colliders.push_back(collider);
		}

		for (int i = 0; i < colliders.size(); i++)
		{
			world.circleColliders.push_back(&colliders[i]);
		}
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		//renderer->camPos = c1.position;
		std::cout << c1.velocity.y << std::endl;
		world.stepNoSpaceDivision(dt);
		for (int i = 0; i < colliders.size(); i++)
		{
			renderer->DrawCircleCollider(&colliders[i], { 1,1,1,1 });
		}
	}
};