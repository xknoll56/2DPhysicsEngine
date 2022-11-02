#pragma once
extern float elapsedTime;

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
		boxCollider.position = { -10,-7 };
		world.boxColliders.push_back(&boxCollider);
		c1.position = { 5,5 };
		c1.radius = 3.25f;
		world.circleColliders.push_back(&c1);
		world.useGravity = false;
		boxCollider.isDynamic = false;
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.stepAll(dt);
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



		//renderer->drawCircleCollider(&c1, { 1,1,1,1 });
		//RayCastHit rch;
		//vec2 dir = { cosf(elapsedTime), sinf(elapsedTime) };
		//if (world.boxRayCast(c1.position, dir, boxCollider, rch))
		//{
		//	renderer->drawLine(c1.position, rch.position, 0.05f, true, { 1,0,0,1 });
		//}
		//else
		//{
		//	renderer->drawLine(c1.position, c1.position + 100.0f*dir, 0.05f, true, {0,1,0,1});
		//}

		ContactInfo ci;
		ci.a = &boxCollider;
		ci.b = &c1;
		if (world.boxCircleOverlap(boxCollider, c1, ci))
		{
			renderer->drawCircleCollider(&c1, { 1,0,0,1 });
			renderer->drawCircle(ci.points[0], 0.05f, 0.0f, { 0,0,0,1 }, false);
			renderer->drawLine(ci.points[0], ci.points[0] - ci.normal * ci.penetration, 0.05f, true, { 0,1,0,1 });
		}
		else
			renderer->drawCircleCollider(&c1, { 1,1,1,1 });

	}
};