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
		boxCollider.angularMomentum = 1;
		boxCollider.halfExtents = { 0.8, 2.0 };
		world.boxColliders.push_back(&boxCollider);
		c1.position = { 5,5 };
		c1.radius = 0.8f;
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
			c1.setVelocity({ 0, 3 });
		if (keys[KEY_J])
			c1.setVelocity({ -3, 0 });
		if (keys[KEY_K])
			c1.setVelocity({ 0, -3 });
		if (keys[KEY_L])
			c1.setVelocity({ 3, 0 });

		if(!world.boxCircleOverlap(boxCollider, c1))
			renderer->drawCircleCollider(&c1, { 1,1,1,1 });
		else
			renderer->drawCircleCollider(&c1, { 1,0,0,1 });

	}
};