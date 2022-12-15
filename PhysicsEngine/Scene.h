#pragma once
extern Vec2 cursorPosition;
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

	Vec2 normalizedScreenCoords()
	{
		return Vec2{ 2.0f * (cursorPosition.x / SCREEN_WIDTH - 0.5f), 2.0f * (0.5f - cursorPosition.y / SCREEN_HEIGHT)/(ASPECT_RATIO)};
	}

	Vec2 screenWorldCoords()
	{
		//std::cout << cursorPosition.x/SCREEN_WIDTH << " " << cursorPosition.y/SCREEN_HEIGHT << std::endl;
		Vec2 norm = normalizedScreenCoords();
		norm.y = norm.y*SCREEN_WIDTH;
		return renderer->camPos + renderer->camWidth * normalizedScreenCoords();
	}
};