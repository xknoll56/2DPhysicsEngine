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


struct CircleCollider : RigidBody
{
	float radius;

	CircleCollider()
	{
		radius = 0.5f;
	}

	CircleCollider(float radius)
	{
		this->radius = radius;
	}
 };


struct BoxCollider : RigidBody
{
	vec2 halfExtents;

	BoxCollider()
	{
		halfExtents = { 0.5f, 0.5f };
	}

	BoxCollider(vec2 halfExtents)
	{
		this->halfExtents = halfExtents;
	}
};
