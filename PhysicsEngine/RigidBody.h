#pragma once
#include <list>
struct RigidBody
{
	float mass;
	float inertia;
	Vec2 position;
	Vec2 velocity;
	Vec2 momentum;
	float angle;
	float angularVelocity;
	float angularMomentum;
	float gravitationalConstant;
	bool useGravity;
	bool isDynamic;
	bool asleep;
	bool resingContact;
	bool lockRotation;
	std::list<Vec2> forces;
	std::list<float> torques;

	Vec2 getLocalX() const
	{
		return Vec2{ cosf(angle), sinf(angle) };
	}

	Vec2 getLocalY() const
	{
		return Vec2{ -sinf(angle), cosf(angle) };
	}

	void setAngleFromRight(Vec2 right)
	{
		//Vec2 up = Tangent(right);
		angle = atanf(right.y / right.x);
	}

	void setAngleFromUp(Vec2 up)
	{
		Vec2 right = -Tangent(up);
		angle = atanf(right.y / right.x);
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
		asleep = false;
		resingContact = false;
		lockRotation = false;
	}

	void addForce(Vec2 force)
	{
		forces.push_back(force);
	}

	void addTorque(float torque)
	{
		torques.push_back(torque);
	}

	void wakeUp()
	{
		resingContact = false;
		asleep = false;
	}

	void step(float dt)
	{
		if (isDynamic && !asleep)
		{
			if (useGravity)
				forces.push_back(Vec2{ 0,gravitationalConstant*mass });
			for (std::list<Vec2>::iterator i = forces.begin(); i != forces.end(); i++)
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
			if(!lockRotation)
				angle += angularVelocity * dt;
		}
	}

	void setVelocity(Vec2 velocity)
	{
		this->momentum = velocity * mass;
		this->velocity = velocity;
	}

	void setAngularVelocity(float angularVelocity)
	{
		this->angularMomentum = angularVelocity * inertia;
		this->angularVelocity = angularVelocity;
	}
};