#pragma once
extern float elapsedTime;

class BoxBoxCollisionScene : Scene
{
public:
	BoxCollider boxCollider;
	BoxCollider boxCollider1;
	SquareSpace* squareSpace;

	BoxBoxCollisionScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		//boxCollider.angularMomentum = 1;
		boxCollider.halfExtents = { 7.5f, 0.5f };
		boxCollider.angle = 3.14159f / 5.0f;
		boxCollider1.halfExtents = { 0.8, 0.5f };
		boxCollider1.setAABB();
		boxCollider1.angle = -3.14159f / 5.0f;
		boxCollider1.position = { 0, -8 };
		world.boxColliders.push_back(&boxCollider);
		world.boxColliders.push_back(&boxCollider1);
		world.useGravity = false;
		boxCollider.angularMomentum = 1.0f;
		//boxCollider.isDynamic = false;
		//boxCollider1.isDynamic = false;
		//squareSpace = new SquareSpace(5, 5, { -20,-20 }, 20);
		//world.squareSpace = squareSpace;
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.stepAll(dt);
		renderer->drawBoxCollider(&boxCollider, { 1,1,1,1 });

		ContactInfo ci;
		if (!world.boxBoxOverlap(boxCollider, boxCollider1, ci))
			renderer->drawBoxCollider(&boxCollider1, { 1,1,1,1 });
		else
			renderer->drawBoxCollider(&boxCollider1, { 1,0,0,1 });
	}
};