#pragma once
#include <list>
struct RigidBody
{
	float mass;
	float inertia;
	vec2 position;
	vec2 velocity;
	vec2 momentum;
	float angle;
	float angularVelocity;
	float angularMomentum;
	float gravitationalConstant;
	bool useGravity;
	bool isDynamic;
	std::list<vec2> forces;
	std::list<float> torques;

	vec2 getLocalX() const
	{
		return vec2{ cosf(angle), sinf(angle) };
	}

	vec2 getLocalY() const
	{
		return vec2{ -sinf(angle), cosf(angle) };
	}
	

	RigidBody()
	{
		mass = 1.0f;
		inertia = 1.0f;
		position = { 0.0f,0.0f };
		velocity = { 0.0f,0.0f };
		angle = 0.0f;
		angularVelocity = 0.0f;
		angularMomentum = 0.0f;
		momentum = { 0.0f,0.0f };
		gravitationalConstant = -9.81f;
		isDynamic = true;
		useGravity = true;
	}

	void addForce(vec2 force)
	{
		forces.push_back(force);
	}

	void addTorque(float torque)
	{
		torques.push_back(torque);
	}

	void step(float dt)
	{
		if (isDynamic)
		{
			if (useGravity)
				forces.push_back(vec2{ 0,gravitationalConstant*mass });
			for (std::list<vec2>::iterator i = forces.begin(); i != forces.end(); i++)
			{
				momentum += *i * dt;
			}
			forces.clear();
			for (std::list<float>::iterator i = torques.begin(); i != torques.end(); i++)
			{
				angularMomentum += *i * dt;
			}
			torques.clear();

			velocity = momentum * (1.0f / mass);
			angularVelocity = angularMomentum / inertia;

			position += velocity * dt;
			angle += angularVelocity * dt;
		}
	}

	void setVelocity(vec2 velocity)
	{
		this->momentum = velocity * mass;
		this->velocity = velocity;
	}
};