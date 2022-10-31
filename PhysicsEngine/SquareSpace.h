#pragma once
struct AABB
{
	vec2 halfExtents;
	vec2 center;
	vec2 bottomLeftExtents;
	vec2 topLeftExtents;
	vec2 topRightExtents;
	vec2 bottomRightExtents;


	AABB()
	{
		halfExtents = { 0.5f,0.5f };
		center = { 0.0f, 0.0f };
	}
	AABB(vec2 halfExtents, vec2 center)
	{
		this->halfExtents = halfExtents;
		this->center = center;
	}

	void setExtents()
	{
		bottomLeftExtents = center - halfExtents;
		topRightExtents = center + halfExtents;
	}
};

class SquareSpace
{
public:
	int xSize;
	int ySize;
	vec2 center;
	std::vector<vec2> squares;

	SquareSpace()
	{

	}

	SquareSpace(vec2 center, int xSize, int ySize)
	{
		//this->center = { (int)center.x, (int)center.y };
		//this->xSize = xSize;
		//this->ySize = ySize;

		//for (int i = center.x - xSize; i <= center.x + xSize; i++)
		//{
		//	for (int j = center.y - ySize; j <= center.y + ySize; j++)
		//	{
		//		squares.push_back({ (float)i, (float)j });
		//	}
		//}
	}
	
};