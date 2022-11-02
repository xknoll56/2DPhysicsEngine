#pragma once
extern float elapsedTime;

class BallRollScene : Scene
{
public:
	CircleCollider circleCollider;
	BoxCollider boxCollider;
	BoxCollider boxCollider1;

	BallRollScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		//boxCollider.angularMomentum = 1;
		boxCollider.halfExtents = { 7.5f, 0.1f };
		boxCollider.angle = 3.14159f / 5.0f;
		boxCollider1.halfExtents = { 7.5f, 0.1f };
		boxCollider1.angle = -3.14159f / 5.0f;
		boxCollider1.position = { -5, -10 };
		world.boxColliders.push_back(&boxCollider);
		world.boxColliders.push_back(&boxCollider1);
		circleCollider.position = { 0,10 };
		world.circleColliders.push_back(&circleCollider);
		world.useGravity = true;
		boxCollider.isDynamic = false;
		boxCollider1.isDynamic = false;
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.step(dt);
		renderer->drawBoxCollider(&boxCollider, { 1,1,1,1 });
		renderer->drawBoxCollider(&boxCollider1, { 1,1,1,1 });
		renderer->drawCircleCollider(&circleCollider, { 1,1,1,1 });

	}
};