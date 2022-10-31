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

enum ColliderType
{
	CIRCLE = 0,
	BOX = 1
};
struct Collider : RigidBody
{
	ColliderType type;
};


struct CircleCollider : Collider
{
	float radius;

	CircleCollider()
	{
		radius = 0.5f;
		type = ColliderType::CIRCLE;
	}

	CircleCollider(float radius)
	{
		this->radius = radius;
		type = ColliderType::CIRCLE;
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
	vec2 halfExtents;

	BoxCollider()
	{
		halfExtents = { 0.5f, 0.5f };
		type = ColliderType::BOX;
	}

	BoxCollider(vec2 halfExtents)
	{
		this->halfExtents = halfExtents;
		type = ColliderType::BOX;
	}
};
