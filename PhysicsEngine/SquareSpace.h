#pragma once


class SquareSpace
{
public:
	float squareSize;
	int divisionsX;
	int divisionsY;
	std::vector<AABB> squares;
	vec2 bottomLeftExtent;
	vec2 topRightExtent;
	float spaceError = 0.25f;


	SquareSpace()
	{

	}

	SquareSpace(float squareSize, int divisionsX, int divisionsY, vec2 bottomLeftExtent)
	{
		this->squareSize = squareSize;
		this->divisionsX = divisionsX;
		this->divisionsY = divisionsY;
		this->bottomLeftExtent = bottomLeftExtent;
		this->topRightExtent = bottomLeftExtent + vec2(divisionsX * squareSize, divisionsY * squareSize);

		squares.reserve(divisionsX * divisionsY);

		for (int i = 0; i < divisionsY; i++)
		{
			for (int j = 0; j < divisionsX; j++)
			{
				vec2 ble = bottomLeftExtent + vec2(squareSize * j, squareSize * i);
				vec2 tre = ble + vec2(squareSize, squareSize);
				AABB aabb(ble, tre, vec2(squareSize * 0.5f, squareSize * 0.5f));
				squares.push_back(aabb);
			}
		}

	}

	SquareSpace(int divisionsX, int divisionsY, vec2 bottomLeftExtent, float farRightExtent) : 
		SquareSpace((farRightExtent -bottomLeftExtent.x)/divisionsX, divisionsX, divisionsY, bottomLeftExtent)
	{

	}

	//-1 will mean outside
	int getContainmentSquareIndex(vec2& point)
	{
		if (point.x < bottomLeftExtent.x || point.y < bottomLeftExtent.y || point.x > topRightExtent.x || point.y > topRightExtent.y)
			return -1;
		vec2 offset = point - bottomLeftExtent;
		offset.x /= squareSize;
		offset.y /= squareSize;
		return (int)(offset.y) * divisionsX + (int)offset.x;
	}

	std::list<int> getContainmentSquareIndices(AABB& aabb)
	{
		std::list<int> indices;

		indices.push_back(getContainmentSquareIndex(aabb.bottomLeftExtent));
		indices.push_back(getContainmentSquareIndex(aabb.bottomRightExtent));
		indices.push_back(getContainmentSquareIndex(aabb.topLeftExtent));
		indices.push_back(getContainmentSquareIndex(aabb.topRightExtent));
		return indices;
	}
	
};