#pragma once

class TestScene : Scene
{
public: 
	CircleCollider c1;
	CircleCollider c2;
	std::vector<CircleCollider> colliders;
	SquareSpace squareSpace;

	TestScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		for (int i = 0; i < 10; i++)
		{
			CircleCollider cc;
			cc.position = vec2{ (float)(rand() % 10 - 5),  (float)(rand() % 10  - 5)};
			cc.momentum = vec2{ (float)(rand() % 10 - 5),  (float)(rand() % 10  - 5)};
			std::cout << cc.velocity.x << std::endl;
			colliders.push_back(cc);
		}
		for(int i = 0; i<colliders.size(); i++)
			world.circleColliders.push_back(&colliders.at(i));
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.step(dt);
		for (int i = 0; i < colliders.size(); i++)
		{
			renderer->DrawCircleCollider(&colliders[i], { 1,1,1,1 });
		}

	}
};