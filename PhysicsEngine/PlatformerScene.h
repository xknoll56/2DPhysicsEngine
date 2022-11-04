#pragma once
extern float elapsedTime;

class PlatformerScene : Scene
{
public:
	std::vector<BoxCollider> terrain;
	//BoxCollider ground;
	BoxCollider player;

	PlatformerScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		BoxCollider ground;
		ground.halfExtents = { 100.0f, 1.0f };
		ground.isDynamic = false;
		//world.boxColliders.push_back(&ground);
		terrain.push_back(ground);

		player.halfExtents = { 0.5f, 0.8f };
		player.position = { 0, 5.0f };
		player.lockRotation = true;
		world.boxColliders.push_back(&player);

		for (int i = 0; i<terrain.size();i++)
			world.boxColliders.push_back(&terrain[i]);


		world.setSquareSpace(5.0f);
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.stepAll(dt);
		drawSquareSpace(*world.squareSpace);

		for (BoxCollider& bc : terrain)
			renderer->drawBoxCollider(&bc, { 0.6,0.3,0.0,1.0f });

		renderer->drawBoxCollider(&player, RGBA::BLUE);
	}
};