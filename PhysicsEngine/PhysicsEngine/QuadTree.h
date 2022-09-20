#pragma once
#include <vector>
struct Quadrant
{
	vec2 position;
	vec2 halfExtents;

	Quadrant(vec2 position, vec2 halfExtents)
	{
		this->position = position;
		this->halfExtents = halfExtents;
		bottomLeftExtent = position - halfExtents;
		topRightExtent = position + halfExtents;
		topLeftExtent = position + vec2(-halfExtents.x, halfExtents.y);
		bottomRightExtent = position + vec2(halfExtents.x, -halfExtents.y);
		children.reserve(4);
		for (int i = 0; i < 4; i++)
		{
			Quadrant q;
			children.push_back(&q);
		}
	}

	Quadrant()
	{

	}

	vec2 bottomLeftExtent;
	vec2 topLeftExtent;
	vec2 topRightExtent;
	vec2 bottomRightExtent;

	std::vector<Quadrant*> children;

};

struct QuadTree
{
	Quadrant* root;
	std::vector<Quadrant> quadrants;

	QuadTree()
	{
		quadrants.reserve(10000);
	}

	QuadTree(int reserve)
	{
		quadrants.reserve(reserve);
	}
	
	void subdivideAllQuadrant(Quadrant* quadrant)
	{
		vec2 halfHalfExtents = quadrant->halfExtents * 0.25f;

		Quadrant q((quadrant->position + quadrant->bottomLeftExtent) * 0.5f, halfHalfExtents);
		quadrants.push_back(q);
		quadrant->children[0] = &quadrants[quadrants.size() - 1];

		Quadrant q1((quadrant->position + quadrant->topLeftExtent) * 0.5f, halfHalfExtents);
		quadrants.push_back(q1);
		quadrant->children[1] = &quadrants[quadrants.size() - 1];

		Quadrant q2((quadrant->position + quadrant->topRightExtent) * 0.5f, halfHalfExtents);
		quadrants.push_back(q2);
		quadrant->children[2] = &quadrants[quadrants.size() - 1];

		Quadrant q3((quadrant->position + quadrant->bottomRightExtent) * 0.5f, halfHalfExtents);
		quadrants.push_back(q3);
		quadrant->children[3] = &quadrants[quadrants.size() - 1];
	}
};