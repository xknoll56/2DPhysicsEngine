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

		Vec2 startPos = aabb.bottomLeftExtent;
		Vec2 endPos = aabb.topRightExtent;
		int startInd = getContainmentSquareIndex(startPos);
		indices.push_back(startInd);
		int endInd = getContainmentSquareIndex(endPos);
		if(endInd!=startInd)
			indices.push_back(endInd);
		for (float x = startPos.x; x < (endPos.x+squareSize); x += squareSize)
		{
			for (float y = startPos.y; y < (endPos.y+squareSize); y += squareSize)
			{
				Vec2 pos = { x, y };
				int ind = getContainmentSquareIndex(pos);
				if (!(std::find(indices.begin(), indices.end(), ind) != indices.end()))
					indices.push_back(ind);
			}
		}

		return indices;
	}
	
};