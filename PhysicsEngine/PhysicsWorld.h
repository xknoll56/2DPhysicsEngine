#pragma once

struct CircleColliderPair
{
	CircleCollider* a;
	CircleCollider* b;
};

struct PhysicsWorld
{
	std::vector<BoxCollider*> boxColliders;
	std::vector<CircleCollider*> circleColliders;
	float restitution = 0.4f;

	bool boxCircleOverlap(const BoxCollider& a, const CircleCollider& b)
	{

	}

	bool circleCircleOverlap(const CircleCollider& a, const CircleCollider& b)
	{
		vec2 dp = b.position - a.position;
		float length = dp.mag();
		float penetration = a.radius + b.radius - length;
		if (penetration > 0)
			return true;
		else
			return false;
	}

	void circleCircleResponse(float dt, CircleCollider& a, CircleCollider& b)
	{
		vec2 dp = b.position - a.position;
		float penetration = dp.mag() - (a.radius + b.radius);
		penetration = fabsf(penetration);
		dp.normalize();
		a.position += dp * (-penetration * 0.5f);
		b.position += dp * (penetration * 0.5f);
		vec2 relMomentum = a.momentum - b.momentum;


		float mag = Dot(dp, relMomentum);
		float restitution = 0.4f;
		if (mag > 0)
		{
			vec2 relMomentumNorm = mag * dp * restitution;
			b.addForce(relMomentumNorm *(1.0f/dt));
			a.addForce(relMomentumNorm * (-1.0f / dt));
		}

		vec2 dpPerp = { -dp.y, dp.x };
		mag = Dot(dpPerp, relMomentum);
		if (mag > 0)
		{
			b.addTorque(mag*restitution * (1.0f / dt));
			a.addTorque(mag*restitution * (-1.0f / dt));
		}

	}

	void step(float dt)
	{
		for (int i = 0; i < circleColliders.size(); i++)
			circleColliders[i]->step(dt);

		for (int i = 0; i < boxColliders.size(); i++)
			boxColliders[i]->step(dt);

		std::vector<CircleColliderPair> circleColliderPairs;

		for (int i = 0; i < circleColliders.size(); i++)
		{
			for (int j = 0; j < circleColliders.size(); j++)
			{
				if (i != j)
				{
					CircleCollider* a = circleColliders[i];
					CircleCollider* b = circleColliders[j];
					if (circleCircleOverlap(*a, *b))
					{

						//if the circles overlap, see if the pair is already found and
						//if not add it to the vector/list
						bool pairFound = false;
						for (int k = 0; k < circleColliderPairs.size(); k++)
						{
							if ((circleColliderPairs[k].a == a && circleColliderPairs[k].b == b) || circleColliderPairs[k].a == b && circleColliderPairs[k].b == a)
							{
								pairFound = true;
								break;
							}
						}

						if (!pairFound)
						{
							circleColliderPairs.push_back(CircleColliderPair{ a,b });
						}
					}
				}
			}
		}

		//handle the collisions
		for (int k = 0; k < circleColliderPairs.size(); k++)
		{
			circleCircleResponse(dt, *circleColliderPairs[k].a, *circleColliderPairs[k].b);
		}
	}
};