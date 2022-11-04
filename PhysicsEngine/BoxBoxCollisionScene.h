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
		boxCollider1.halfExtents = { 0.5, 0.5f };
		boxCollider1.setAABB();
		boxCollider1.angle = -3.14159f / 5.0f;
		boxCollider1.position = { 0, 10 };
		boxCollider1.momentum = { 0.2, 0 };
		boxCollider1.angularMomentum = 1;
		//boxCollider1.lockRotation = true;
		world.boxColliders.push_back(&boxCollider);
		world.boxColliders.push_back(&boxCollider1);
		world.useGravity = true;
		//boxCollider.angularMomentum = 1.0f;
		boxCollider.isDynamic = false;
		//boxCollider1.isDynamic = false;
		//squareSpace = new SquareSpace(5, 5, { -20,-20 }, 20);
		//world.squareSpace = squareSpace;
		world.setSquareSpace(5.0f);
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.step(dt);
		drawSquareSpace(*world.squareSpace);
		boxCollider1.setAABB();
		renderer->drawAABB(boxCollider1.aabb, RGBA::RED);
		renderer->drawBoxCollider(&boxCollider, { 1,1,1,1 });

		//boxCollider1.setVelocity({ 0,0 });
		//if (keys[KEY_I])
		//	boxCollider1.setVelocity({ 0, 3 });
		//if (keys[KEY_J])
		//	boxCollider1.setVelocity({ -3, 0 });
		//if (keys[KEY_K])
		//	boxCollider1.setVelocity({ 0, -3 });
		//if (keys[KEY_L])
		//	boxCollider1.setVelocity({ 3, 0 });

		ContactInfo ci;
		RayCastHit rch;
		//Vec2 dir = { 0, -1 };
		//if (world.boxRayCast(boxCollider1.position, dir, boxCollider, rch))
		//	renderer->drawLine(boxCollider1.position, rch.position, 0.05f, true, RGBA::RED);
		//else
		//	renderer->drawLine(boxCollider1.position, boxCollider1.position+dir*100.0f, 0.05f, true, RGBA::GREEN);
		renderer->drawUnitDirs(boxCollider1.position, boxCollider1.angle);
		if (!world.boxBoxOverlap(boxCollider, boxCollider1, ci))
			renderer->drawBoxCollider(&boxCollider1, RGBA::WHITE);
		else
		{
			renderer->drawBoxCollider(&boxCollider1, RGBA::RED);
			renderer->drawLine(boxCollider1.position, boxCollider1.position + ci.normal, 0.05f, true, RGBA::BLUE);
			for (int i = 0; i < ci.points.size(); i++)
			{
				renderer->drawCircle(ci.points[i], 0.1f, 0.0f, RGBA::BLUE, false);
			}
		}

	}
};