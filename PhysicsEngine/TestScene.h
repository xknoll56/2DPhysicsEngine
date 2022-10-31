#pragma once

class TestScene : Scene
{
public: 
	CircleCollider c1;
	CircleCollider c2;
	BoxCollider boxCollider;
	SquareSpace squareSpace;

	TestScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		//for (int i = 0; i < 10; i++)
		//{
		//	CircleCollider cc;
		//	cc.position = vec2{ (float)(rand() % 10 - 5),  (float)(rand() % 10  - 5)};
		//	cc.momentum = vec2{ (float)(rand() % 10 - 5),  (float)(rand() % 10  - 5)};
		//	std::cout << cc.velocity.x << std::endl;
		//	colliders.push_back(cc);
		//}
		//for(int i = 0; i<colliders.size(); i++)
		//	world.circleColliders.push_back(&colliders.at(i));
		boxCollider.angularMomentum = 1;
		boxCollider.halfExtents = { 0.8, 2.0 };
		world.boxColliders.push_back(&boxCollider);
		c1.position = { 5,5 };
		world.circleColliders.push_back(&c1);
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.step(dt);
		renderer->drawBoxCollider(&boxCollider, { 1,1,1,1 });
		renderer->drawUnitDirs(boxCollider.position, boxCollider.angle);

		c1.setVelocity({ 0,0 });
		if (keys[KEY_I])
			c1.setVelocity({ 0, 1 });
		if (keys[KEY_J])
			c1.setVelocity({ -1, 0 });
		if (keys[KEY_K])
			c1.setVelocity({ 0, -1 });
		if (keys[KEY_L])
			c1.setVelocity({ 1, 0 });

		if(!world.boxCircleOverlap(boxCollider, c1))
			renderer->drawCircleCollider(&c1, { 1,1,1,1 });
		else
			renderer->drawCircleCollider(&c1, { 1,0,0,1 });
		//renderer->drawLine(boxCollider.position, boxCollider.position + boxCollider.getLocalX(), 0.05f, true, { 0,0,1,1 });
		//renderer->drawLine(boxCollider.position, boxCollider.position + boxCollider.getLocalY(), 0.05f, true, { 1,0,0,1 });
		//for (int i = 0; i < colliders.size(); i++)
		//{
		//	renderer->DrawCircleCollider(&colliders[i], { 1,1,1,1 });
		//}

	}
};