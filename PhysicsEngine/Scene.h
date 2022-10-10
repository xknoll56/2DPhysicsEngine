#pragma once
class Scene
{
public:
	Renderer* renderer;
	PhysicsWorld world;

	virtual void setup()
	{

	}

	virtual void update(float dt)
	{

	}

	void moveCamera(float speed, float dt)
	{
		if (keys[KEY_W])
			renderer->camPos += vec2(0, 5.0f * dt);
		if (keys[KEY_A])
			renderer->camPos += vec2(-5.0f * dt, 0);
		if (keys[KEY_S])
			renderer->camPos += vec2(0, -5.0f * dt);
		if (keys[KEY_D])
			renderer->camPos += vec2(5.0f * dt, 0);
	}
};