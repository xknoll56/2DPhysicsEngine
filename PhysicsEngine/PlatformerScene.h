#pragma once
extern float elapsedTime;

class PlatformerScene : Scene, CollisionObserver
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
		player.enableRestingContact = false;
		player.callbacks.push_back(this);
		//player.useGravity = false;
		world.boxColliders.push_back(&player);

		BoxCollider platform;
		platform.halfExtents = { 5.0f, 0.1f };
		platform.position = { 10, 5 };
		platform.isDynamic = false;
		terrain.push_back(platform);

		for (int i = 0; i<terrain.size();i++)
			world.boxColliders.push_back(&terrain[i]);


		world.setSquareSpace(5.0f);
		//world.useGravity = false;
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.step(dt);
		drawSquareSpace(*world.squareSpace);

		Vec2 vel = { 0.0f, player.velocity.y };
		float speed = 3.0f;
		float jumpSpeed = 10.0f;
		if (keysDown[VK_SPACE])
			vel += {0, jumpSpeed};
		if (keys[VK_LEFT])
			vel += {-speed, 0.0f};
		if (keys[VK_RIGHT])
			vel += {speed, 0.0f};
		player.setVelocity(vel);

		for (BoxCollider& bc : terrain)
			renderer->drawBoxCollider(&bc, { 0.6,0.3,0.0,1.0f });

		renderer->drawBoxCollider(&player, RGBA::BLUE);
	}

	void onOverlap(ContactInfo& ci) override
	{
		
	}
};