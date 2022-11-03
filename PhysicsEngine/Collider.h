#pragma once

struct AABB
{
	Vec2 halfExtents;
	Vec2 center;
	Vec2 bottomLeftExtent;
	Vec2 topRightExtent;
	Vec2 topLeftExtent;
	Vec2 bottomRightExtent;
	bool isOutside = false;


	AABB()
	{
		halfExtents = { 0.5f,0.5f };
		center = { 0.0f, 0.0f };
	}
	AABB(Vec2 halfExtents, Vec2 center)
	{
		this->halfExtents = halfExtents;
		this->center = center;
	}

	AABB(Vec2 bottomLeftExtent, Vec2 topRightExtent, Vec2 halfExtents)
	{
		this->bottomLeftExtent = bottomLeftExtent;
		this->topRightExtent = topRightExtent;
		this->halfExtents = halfExtents;
		this->topLeftExtent = Vec2{ bottomLeftExtent.x, bottomLeftExtent.y + 2 * halfExtents.y };
		this->bottomRightExtent = Vec2{ topRightExtent.x, topRightExtent.y - 2 * halfExtents.y };
		center = bottomLeftExtent + topRightExtent;
		center.x *= 0.5f;
		center.y *= 0.5f;

	}

	void setExtents()
	{
		bottomLeftExtent = center - halfExtents;
		topRightExtent = center + halfExtents;
		topLeftExtent = Vec2{ bottomLeftExtent.x, bottomLeftExtent.y + 2 * halfExtents.y };
		bottomRightExtent = Vec2{ topRightExtent.x, topRightExtent.y - 2 * halfExtents.y };
	}
};

enum ColliderType
{
	CIRCLE = 0,
	BOX = 1,
	NONE = 2
};
struct Collider : RigidBody
{
	ColliderType type = ColliderType::NONE;
	AABB aabb;

	virtual ColliderType getType()
	{
		return type;
	}

	void setAABB()
	{
		aabb.center = position;
		aabb.setExtents();
	}
};


struct CircleCollider : Collider
{
	float radius;
	

	CircleCollider()
	{
		radius = 0.5f;
		type = ColliderType::CIRCLE;
		aabb.halfExtents = { 0.5f, 0.5f };
	}

	CircleCollider(float radius)
	{
		this->radius = radius;
		type = ColliderType::CIRCLE;
		aabb.halfExtents = { radius, radius };
	}

 };

enum BoxSide
{
	RIGHT = 0,
	UP,
	LEFT,
	DOWN
};
struct BoxCollider : Collider
{
	Vec2 halfExtents;

	BoxCollider()
	{
		halfExtents = { 0.5f, 0.5f };
		type = ColliderType::BOX;
		aabb.halfExtents = { 0.5f, 0.5f };
	}

	BoxCollider(Vec2 halfExtents)
	{
		this->halfExtents = halfExtents;
		type = ColliderType::BOX;
		aabb.halfExtents = { fmaxf(halfExtents.x, halfExtents.y), fmaxf(halfExtents.x, halfExtents.y) };
	}

	void setAABBHalfExtents()
	{
		aabb.halfExtents = { fmaxf(halfExtents.x, halfExtents.y), fmaxf(halfExtents.x, halfExtents.y) };
	}
};
