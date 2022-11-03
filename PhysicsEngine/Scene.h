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
			renderer->camPos += Vec2(0, 5.0f * dt);
		if (keys[KEY_A])
			renderer->camPos += Vec2(-5.0f * dt, 0);
		if (keys[KEY_S])
			renderer->camPos += Vec2(0, -5.0f * dt);
		if (keys[KEY_D])
			renderer->camPos += Vec2(5.0f * dt, 0);
		if (keys[KEY_Q])
			renderer->camWidth -= 5.0f*dt;
		if (keys[KEY_E])
			renderer->camWidth += 5.0f*dt;
	}

	void drawSquareSpace(SquareSpace& squareSpace)
	{
		for (int i = 0; i < squareSpace.squares.size(); i++)
		{
			AABB aabb = squareSpace.squares[i].aabb;
			renderer->drawBox(aabb.center, aabb.halfExtents, 0.0f, false, { 0,1,0,1 });
		}
	}
};