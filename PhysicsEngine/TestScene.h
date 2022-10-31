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
		c1.radius = 3.25f;
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



		vec2 dir = { 1, 0.5 };
		dir.normalize();
		RayCastHit hit;
		if (!world.circleRayCast({ 0,0 }, dir, c1, hit))
			renderer->drawLine({ 0,0 }, dir * 20.0f, 0.05, true, { 1,0,0,1 });
		else
			renderer->drawLine({ 0,0 }, hit.position, 0.05, true, { 0,0,1,1 });

	}
};