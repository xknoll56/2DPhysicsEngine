#pragma once


class SquareSpace
{
public:
	struct Square
	{
		AABB aabb;
		std::list<Collider*> colliders;
	};
	float squareSize;
	int divisionsX;
	int divisionsY;
	std::vector<Square> squares;
	Square outside;
	Vec2 bottomLeftExtent;
	Vec2 topRightExtent;
	float spaceError = 0.25f;


	SquareSpace()
	{

	}

	SquareSpace(float squareSize, int divisionsX, int divisionsY, Vec2 bottomLeftExtent)
	{
		this->squareSize = squareSize;
		this->divisionsX = divisionsX;
		this->divisionsY = divisionsY;
		this->bottomLeftExtent = bottomLeftExtent;
		this->topRightExtent = bottomLeftExtent + Vec2(divisionsX * squareSize, divisionsY * squareSize);

		squares.reserve(divisionsX * divisionsY);

		for (int i = 0; i < divisionsY; i++)
		{
			for (int j = 0; j < divisionsX; j++)
			{
				Vec2 ble = bottomLeftExtent + Vec2(squareSize * j, squareSize * i);
				Vec2 tre = ble + Vec2(squareSize, squareSize);
				AABB aabb(ble, tre, Vec2(squareSize * 0.5f, squareSize * 0.5f));
				Square square;
				square.aabb = aabb;
				squares.push_back(square);
			}
		}

	}

	SquareSpace(int divisionsX, int divisionsY, Vec2 bottomLeftExtent, float farRightExtent) : 
		SquareSpace((farRightExtent -bottomLeftExtent.x)/divisionsX, divisionsX, divisionsY, bottomLeftExtent)
	{

	}

	//-1 will mean outside
	int getContainmentSquareIndex(Vec2& point)
	{
		if (point.x < bottomLeftExtent.x || point.y < bottomLeftExtent.y || point.x > topRightExtent.x || point.y > topRightExtent.y)
			return -1;
		Vec2 offset = point - bottomLeftExtent;
		offset.x /= squareSize;
		offset.y /= squareSize;
		return (int)(offset.y) * divisionsX + (int)offset.x;
	}

	std::list<int> getContainmentSquareIndices(AABB& aabb)
	{
		std::list<int> indices;
		int i = getContainmentSquareIndex(aabb.bottomLeftExtent);
		if(i>=0)
			indices.push_back(i);
		i = getContainmentSquareIndex(aabb.bottomRightExtent);
		if(!(std::find(indices.begin(), indices.end(), i) != indices.end()))
			indices.push_back(i);
		i = getContainmentSquareIndex(aabb.topLeftExtent);
		if(!(std::find(indices.begin(), indices.end(), i) != indices.end()))
			indices.push_back(i);
		i = getContainmentSquareIndex(aabb.topRightExtent);
		if(!(std::find(indices.begin(), indices.end(), i) != indices.end()))
			indices.push_back(i);
		return indices;
	}
	
};