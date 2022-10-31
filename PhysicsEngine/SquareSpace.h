#pragma once


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