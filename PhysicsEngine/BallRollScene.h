#pragma once
extern float elapsedTime;

class BallRollScene : Scene
{
public:
	CircleCollider circleCollider;
	BoxCollider boxCollider;
	BoxCollider boxCollider1;
	SquareSpace* squareSpace;

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
		squareSpace = new SquareSpace(2, 2, { -20,-20 }, 20);
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.step(dt);
		renderer->drawBoxCollider(&boxCollider, { 1,1,1,1 });
		renderer->drawBoxCollider(&boxCollider1, { 1,1,1,1 });
		renderer->drawCircleCollider(&circleCollider, { 1,1,1,1 });
		drawSquareSpace(*squareSpace);


		circleCollider.setAABB();
		renderer->drawBox(circleCollider.aabb.center, circleCollider.aabb.halfExtents, 0.0f, false, { 0,1,0,1 });


		std::list<int> sis = squareSpace->getContainmentSquareIndices(circleCollider.aabb);
		for (std::list<int>::const_iterator it = sis.begin(); it != sis.end(); it++)
		{
			if (*it >= 0)
			{
				AABB curSquare = squareSpace->squares[*it];
				renderer->drawAABB(curSquare, { 1,0,0,1 });
			}
		}

	}
};