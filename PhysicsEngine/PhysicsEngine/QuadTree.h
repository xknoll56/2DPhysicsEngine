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
			parent = this;
		}
	}

	Quadrant operator=(const Quadrant& other)
	{
		return Quadrant(other.position, other.halfExtents);
	}

	Quadrant()
	{

	}

	vec2 bottomLeftExtent;
	vec2 topLeftExtent;
	vec2 topRightExtent;
	vec2 bottomRightExtent;
	bool set = false;
	

	Quadrant* parent = nullptr;
	std::vector<Quadrant*> children;
	RigidBody* rigidBody = nullptr;
};

struct QuadTree
{
	Quadrant* root;
	Quadrant* outside;
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

		Quadrant q1((quadrant->position + quadrant->bottomRightExtent) * 0.5f, halfHalfExtents);
		quadrants.push_back(q1);
		quadrant->children[1] = &quadrants[quadrants.size() - 1];

		Quadrant q2((quadrant->position + quadrant->topLeftExtent) * 0.5f, halfHalfExtents);
		quadrants.push_back(q2);
		quadrant->children[2] = &quadrants[quadrants.size() - 1];

		Quadrant q3((quadrant->position + quadrant->topRightExtent) * 0.5f, halfHalfExtents);
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

	bool insert(Quadrant* q, RigidBody* rb)
	{
		if (!pointIsInQuadrant(*q, rb->position))
		{
			return false;
		}
		bool samePos = false;
		if (q->rigidBody != nullptr)
		{
			if(q->rigidBody->position == rb->position)
				samePos = true;
		}

		if (!q->set)
		{
			q->rigidBody = rb;
			subdivideAllQuadrant(q);
			q->set = true;
		}
		else
		{

			QuadrantLocation qlChild = quadrantizePoint(q, rb->position);
			insert(q->children[(int)qlChild], rb);

			if (q->rigidBody != nullptr)
			{
				QuadrantLocation qlChild1 = quadrantizePoint(q, q->rigidBody->position);
				insert(q->children[(int)qlChild1], q->rigidBody);
				q->rigidBody = nullptr;
			}

		}

		return true;
	}

	enum QuadrantLocation
	{
		bottomLeft = 0,
		bottomRight = 1,
		topLeft = 2,
		topRight = 3
	};

	QuadrantLocation quadrantizePoint(Quadrant* q, vec2 point)
	{
		int ret = 0b00;
		if (point.x >= q->position.x)
			ret |= 0b01;
		if (point.y >= q->position.y)
			ret |= 0b10;

		return (QuadrantLocation)ret;
	}



	void remove(RigidBody* rb, Quadrant* root)
	{
		if (root->set)
		{
			if (rb == root->rigidBody)
			{
				root->rigidBody = nullptr;
				bool found = false;
				for (int i = 0; i < root->parent->children.size(); i++)
				{
					if (root->parent->children[i]->rigidBody != nullptr)
					{
						found = true;
						break;
					}
				}
				if (!found)
					*root->parent = Quadrant(root->parent->position, root->parent->halfExtents);
			}
			else
			{
				QuadrantLocation loc = quadrantizePoint(root, rb->position);
				remove(rb, root->children[(int)loc]);
			}
		}
	}
};