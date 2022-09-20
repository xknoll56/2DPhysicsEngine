#pragma once
#include <vector>

struct RigidBody
{
	vec2 position;
	vec2 velocity;
	float angle;
	float angularVelocity;
};
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
	bool set = false;
#define MAX_SUBS 10000
	

	std::vector<Quadrant*> children;
	std::vector<RigidBody*> rigidBodies;
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
		vec2 halfHalfExtents =  0.5f*quadrant->halfExtents;
		
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

	bool pointIsInQuadrant(Quadrant& q, vec2 point)
	{
		if (point.x >= q.bottomLeftExtent.x && point.x < q.topRightExtent.x)
		{
			if (point.y >= q.bottomLeftExtent.y && point.y < q.topRightExtent.y)
				return true;
		}

		return false;
	}

	void insert(Quadrant* q, RigidBody* rb)
	{
		if (!pointIsInQuadrant(*q, rb->position))
		{
			return;
		}
		bool samePos = false;
		for (int i = 0; i < q->rigidBodies.size(); i++)
		{
			if (q->rigidBodies[i]->position == rb->position)
			{
				samePos = true;
				break;
			}
		}
		if (q->rigidBodies.size()==0 || samePos)
		{
			q->rigidBodies.push_back(rb);
		}
		else
		{
			if (!q->set)
			{
				subdivideAllQuadrant(q);
				q->set = true;
			}

			for (int i = 0; i < q->children.size(); i++)
			{
				Quadrant* child = q->children[i];
				insert(child, rb);
				for (int i = 0; i < q->rigidBodies.size(); i++)
				{
					insert(child, q->rigidBodies[i]);
				}
			}
		}
	}

	RigidBody* get(Quadrant* root)
	{

	}
};