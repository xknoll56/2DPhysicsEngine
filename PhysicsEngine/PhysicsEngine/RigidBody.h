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
	std::list<vec2> forces;
	std::list<float> torques;

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
		for (std::list<vec2>::iterator i = forces.begin(); i!=forces.end(); i++)
		{
			momentum += *i*dt;
		}
		forces.clear();
		for (std::list<float>::iterator i = torques.begin(); i != torques.end(); i++)
		{
			angularMomentum += *i*dt;
		}
		forces.clear();

		velocity = momentum * (1.0f/mass);
		angularVelocity = angularMomentum / inertia;

		position += velocity * dt;
		angle += angularVelocity * dt;
	}

	void setVelocity(vec2 velocity)
	{
		this->momentum = velocity * mass;
		this->velocity = velocity;
	}
};