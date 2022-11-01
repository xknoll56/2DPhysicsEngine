#pragma once

struct RayCastHit
{
	vec2 position;
	vec2 normal;
	float dist;
};

struct ContactInfo
{
	std::vector<vec2> points;
	vec2 normal;
	float penetration;
	Collider* a;
	Collider* b;
};

struct PhysicsWorld
{
	std::vector<BoxCollider*> boxColliders;
	std::vector<CircleCollider*> circleColliders;
	std::vector<ContactInfo> circleColliderPairs;
	std::vector<ContactInfo> circleBoxColliderPairs;
	float restitution = 0.4f;
	bool useGravity = true;

	bool circleRayCast(vec2 position, vec2 direction, const CircleCollider& cc, RayCastHit& rch)
	{
		direction.normalize();
		vec2 dirPerp = { -direction.y, direction.x };
		vec2 dp = cc.position - position;

		if (dp.mag() > cc.radius)
		{
			float distDir = (Dot(direction, dp));
			if (distDir >= 0)
			{
				float distPerp = fabsf(Dot(dirPerp, dp));

				if (distPerp < cc.radius)
				{
					float theta = acosf(distPerp / cc.radius);
					rch.dist = distDir - distPerp * tanf(theta);
					rch.position = position + direction * rch.dist;
					rch.normal = rch.position - cc.position;
					rch.normal.normalize();
					return true;
				}
			}
		}

		return false;
	}

	//p point 1
	//r direction of point 1
	//q point 2
	//s direction of point 2
	bool lineLineIntersection(vec2& intersection, vec2 p, vec2 r, vec2 q, vec2 s)
	{
		float denom = Cross(r, s);
		if (denom == std::numeric_limits<float>::infinity())
			return false;
		float t = Cross(q - p, s) / Cross(r, s);
		float u = Cross(q - p, r) / Cross(r, s);

		intersection = p + t * r;
		if(t>=0.0f && u>=0.0f)
			return true;
		return false;
	}

	bool boxRayCast(vec2 position, vec2 direction, const BoxCollider& bc, RayCastHit& rch)
	{
		bool doesHit = false;
		direction.normalize();
		vec2 right = bc.getLocalX();
		vec2 up = bc.getLocalY();
		float minDist = std::numeric_limits<float>::infinity();

		vec2 bottomLeftPos = bc.position + -bc.halfExtents.x * right - bc.halfExtents.y * up;
		vec2 topRightPos = bc.position + bc.halfExtents.x * right + bc.halfExtents.y * up;

		vec2 intersection;
		if (lineLineIntersection(intersection, position, direction, bottomLeftPos, up))
		{
			vec2 pointOnLine = intersection - bottomLeftPos;
			if (pointOnLine.mag() <= bc.halfExtents.y * 2.0f)
			{
				minDist = (intersection - position).mag();
				rch.dist = minDist;
				rch.position = intersection;
				rch.normal = -right;
				doesHit = true;
			}
		}

		if (lineLineIntersection(intersection, position, direction, bottomLeftPos, right))
		{
			vec2 pointOnLine = intersection - bottomLeftPos;
			if (pointOnLine.mag() <= bc.halfExtents.x*2.0f)
			{
				float dist = (intersection - position).mag();
				if (dist < minDist)
				{
					minDist = dist;
					rch.dist = minDist;
					rch.position = intersection;
					rch.normal = -up;
					doesHit = true;
				}
			}
		}

		if (lineLineIntersection(intersection, position, direction, topRightPos, -up))
		{
			vec2 pointOnLine = intersection - topRightPos;
			if (pointOnLine.mag() <= bc.halfExtents.y * 2.0f)
			{
				float dist = (intersection - position).mag();
				if (dist < minDist)
				{
					minDist = dist;
					rch.dist = minDist;
					rch.position = intersection;
					rch.normal = right;
					doesHit = true;
				}
			}
		}

		if (lineLineIntersection(intersection, position, direction, topRightPos, -right))
		{
			vec2 pointOnLine = intersection - topRightPos;
			if (pointOnLine.mag() <= bc.halfExtents.x * 2.0f)
			{
				float dist = (intersection - position).mag();
				if (dist < minDist)
				{
					minDist = dist;
					rch.dist = minDist;
					rch.position = intersection;
					rch.normal = up;
					doesHit = true;
				}
			}
		}

		return doesHit;
	}


	bool boxCircleOverlap(const BoxCollider& a, const CircleCollider& b, ContactInfo& ci)
	{
		vec2 right = a.getLocalX();
		vec2 up = a.getLocalY();
		vec2 dp = b.position - a.position;

		float rightDist = (Dot(right, dp));
		float upDist = (Dot(up, dp));
		
		float penetration = fabsf(rightDist) - (a.halfExtents.x + b.radius);
		if (penetration > 0)
			return false;
		ci.penetration = penetration;
		bool horizontal = true;

		penetration = fabsf(upDist) - (a.halfExtents.y + b.radius);
		if (penetration > 0)
			return false;
		if (penetration > ci.penetration)
		{
			ci.penetration = penetration;
			horizontal = false;
		}

		RayCastHit rch;
		if (horizontal)
		{
			if (rightDist > 0)
			{
				//b is to right of box a
				if (boxRayCast(b.position, -right, a, rch))
				{
					ci.normal = right;
					ci.points.push_back(rch.position);
					ci.penetration = b.radius - (rch.position - b.position).mag();
					return true;
				}
			}
			else
			{
				//b is to left of box a
				if (boxRayCast(b.position, right, a, rch))
				{
					ci.points.push_back(rch.position);
					ci.normal = -right;
					ci.penetration = b.radius - (rch.position - b.position).mag();
					return true;
				}
			}
		}
		else
		{
			if (upDist > 0)
			{
				//b is above box a
				if (boxRayCast(b.position, -up, a, rch))
				{
					ci.points.push_back(rch.position);
					ci.normal = up;
					ci.penetration = b.radius - (rch.position - b.position).mag();
					return true;
				}
			}
			else
			{
				//b is beneath box a
				if (boxRayCast(b.position, up, a, rch))
				{
					ci.points.push_back(rch.position);
					ci.normal = -up;
					ci.penetration = b.radius - (rch.position - b.position).mag();
					return true;
				}
			}
		}

		vec2 closestVert = a.position + copysignf(1.0f, rightDist) * right * a.halfExtents.x + copysignf(1.0f, upDist) * up * a.halfExtents.y;
		if ((closestVert - b.position).mag() <= b.radius)
		{
			//the closest vert is inside the circle
			ci.points.push_back(closestVert);
			ci.normal = b.position - closestVert;
			ci.penetration = b.radius - ci.normal.mag();
			ci.normal.normalize();
			return true;
		}

		return false;
	}

	void boxCircleResponse(ContactInfo& ci, float dt)
	{
		BoxCollider& a = *dynamic_cast<BoxCollider*>(ci.a);
		CircleCollider& b = *dynamic_cast<CircleCollider*>(ci.b);
		if(a.isDynamic)
			a.position -= ci.penetration * 0.5f * ci.normal;
		if(b.isDynamic)
			b.position += ci.penetration * 0.5f * ci.normal;

		float epsilon = 0.5f;

		vec2 ra = ci.points[0] - a.position;
		vec2 rb = ci.points[0] - b.position;
		vec2 pa = a.velocity + a.angularVelocity * Tangent(ra);
		vec2 pb = b.velocity + b.angularVelocity * Tangent(rb);

		float vRel = Dot(ci.normal, pb - pa);
		float numerator = (1 - epsilon) * vRel;

		float t1 = 1 / a.mass;
		float t2 = 1 / b.mass;
		float t3 = ra.mag() * ra.mag() / a.inertia;
		float t4 = rb.mag() * rb.mag() / b.inertia;

		float j = numerator / (t1 + t2 + t3 + t4);
		vec2 force = ci.normal * j *(1.0f/ dt);

		a.addForce(force);
		float torqueA = Cross(ra, force);
		a.addTorque(torqueA);

		b.addForce(-force);
		vec2 vn = Dot(ci.normal, b.velocity) * ci.normal;
		vec2 vt = b.velocity - vn;
		float torqueB = -copysign(1.0f,Cross(vt, ci.normal))*rb.mag()*force.mag();
		b.addTorque(torqueB);
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
		{
			circleColliders[i]->useGravity = useGravity;
			circleColliders[i]->step(dt);
		}

		for (int i = 0; i < boxColliders.size(); i++)
		{
			boxColliders[i]->useGravity = useGravity;
			boxColliders[i]->step(dt);
		}

		circleColliderPairs.clear();
		circleBoxColliderPairs.clear();

		//handle all circle circle collision
		for (int i = 0; i < circleColliders.size(); i++)
		{
			for (int j = 0; j < circleColliders.size(); j++)
			{
				if (i != j)
				{
					CircleCollider* a = circleColliders[i];
					CircleCollider* b = circleColliders[j];
					ContactInfo ci;
					ci.a = a;
					ci.b = b;
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
							circleColliderPairs.push_back(ci);
						}
					}
				}
			}
		}

		//handle all circle box collider collisions
		for (int i = 0; i < boxColliders.size(); i++)
		{
			for (int j = 0; j < circleColliders.size(); j++)
			{
				BoxCollider* a = boxColliders[i];
				CircleCollider* b = circleColliders[j];
				ContactInfo ci;
				ci.a = a;
				ci.b = b;
				if (boxCircleOverlap(*a, *b, ci))
				{
					circleBoxColliderPairs.push_back(ci);
				}

			}
		}

		//handle the collisions
		for (int k = 0; k < circleColliderPairs.size(); k++)
		{
			CircleCollider& ac = *dynamic_cast<CircleCollider*>(circleColliderPairs[k].a);
			CircleCollider& bc = *dynamic_cast<CircleCollider*>(circleColliderPairs[k].b);
			circleCircleResponse(dt, ac, bc);
		}

		for (int k = 0; k < circleBoxColliderPairs.size(); k++)
		{
			boxCircleResponse(circleBoxColliderPairs[k], dt);
		}
	}
};