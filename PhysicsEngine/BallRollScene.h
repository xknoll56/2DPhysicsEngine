#pragma once
extern float elapsedTime;

class BallRollScene : Scene
{
public:
	CircleCollider circleCollider;
	BoxCollider boxCollider;

	BallRollScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		//boxCollider.angularMomentum = 1;
		boxCollider.halfExtents = { 7.5f, 0.1f };
		boxCollider.angle = 3.14159f / 5.0f;
		world.boxColliders.push_back(&boxCollider);
		circleCollider.position = { 0,10 };
		world.circleColliders.push_back(&circleCollider);
		world.useGravity = true;
		boxCollider.isDynamic = false;
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.step(dt);
		renderer->drawBoxCollider(&boxCollider, { 1,1,1,1 });
		renderer->drawCircleCollider(&circleCollider, { 1,1,1,1 });

	}
};