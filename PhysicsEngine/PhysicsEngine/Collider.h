#pragma once
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