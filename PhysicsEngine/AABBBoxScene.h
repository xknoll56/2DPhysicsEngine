#pragma once
class AABBBoxScene : Scene
{
public:
	BoxCollider boxCollider;
	SquareSpace squareSpace;
	bool pause = false;

	AABBBoxScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		boxCollider.angularMomentum = 1;
		boxCollider.halfExtents = { 7.0f, 0.5f };
		world.boxColliders.push_back(&boxCollider);
		world.useGravity = false;
		boxCollider.setAABB();
		
	}

	void update(float dt)
	{


		moveCamera(5.0f, dt);
		if(!pause)
			world.stepAll(dt/5.0f);

		if (keysDown[KEY_T])
			pause = !pause;

		renderer->drawBoxCollider(&boxCollider, { 1,1,1,1 });
		renderer->drawUnitDirs(boxCollider.position, boxCollider.angle);

		boxCollider.setAABB();
		renderer->drawAABB(boxCollider.aabb, { 1,0,0,1 });
	}
};