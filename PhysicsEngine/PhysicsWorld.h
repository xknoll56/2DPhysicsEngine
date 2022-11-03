#pragma once

struct RayCastHit
{
	Vec2 position;
	Vec2 normal;
	float dist;
};

struct ContactInfo
{
	std::vector<Vec2> points;
	Vec2 normal;
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
	float epsilon = 0.5f;
	bool useGravity = true;
	SquareSpace* squareSpace;

	PhysicsWorld()
	{

	}

	PhysicsWorld(int divisionsX, int divisionsY, Vec2 bottomLeftExtent, float farRightExtent)
	{
		
	}

	bool circleRayCast(Vec2 position, Vec2 direction, const CircleCollider& cc, RayCastHit& rch)
	{
		direction.normalize();
		Vec2 dirPerp = { -direction.y, direction.x };
		Vec2 dp = cc.position - position;

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
	bool lineLineIntersection(Vec2& intersection, Vec2 p, Vec2 r, Vec2 q, Vec2 s)
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

	bool boxRayCast(Vec2 position, Vec2 direction, const BoxCollider& bc, RayCastHit& rch)
	{
		bool doesHit = false;
		direction.normalize();
		Vec2 right = bc.getLocalX();
		Vec2 up = bc.getLocalY();
		float minDist = std::numeric_limits<float>::infinity();

		Vec2 bottomLeftPos = bc.position + -bc.halfExtents.x * right - bc.halfExtents.y * up;
		Vec2 topRightPos = bc.position + bc.halfExtents.x * right + bc.halfExtents.y * up;

		Vec2 intersection;
		if (lineLineIntersection(intersection, position, direction, bottomLeftPos, up))
		{
			Vec2 pointOnLine = intersection - bottomLeftPos;
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
			Vec2 pointOnLine = intersection - bottomLeftPos;
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
			Vec2 pointOnLine = intersection - topRightPos;
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
			Vec2 pointOnLine = intersection - topRightPos;
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
		Vec2 right = a.getLocalX();
		Vec2 up = a.getLocalY();
		Vec2 dp = b.position - a.position;

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

		Vec2 closestVert = a.position + copysignf(1.0f, rightDist) * right * a.halfExtents.x + copysignf(1.0f, upDist) * up * a.halfExtents.y;
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
		BoxCollider& a = *static_cast<BoxCollider*>(ci.a);
		CircleCollider& b = *static_cast<CircleCollider*>(ci.b);
		if(a.isDynamic)
			a.position -= ci.penetration * 0.5f * ci.normal;
		if(b.isDynamic)
			b.position += ci.penetration * 0.5f * ci.normal;


		Vec2 ra = ci.points[0] - a.position;
		Vec2 rb = ci.points[0] - b.position;
		Vec2 pa = a.velocity + a.angularVelocity * Tangent(ra);
		Vec2 pb = b.velocity + b.angularVelocity * Tangent(rb);

		float vRel = Dot(ci.normal, pb - pa);
		float numerator = (1 - epsilon) * vRel;

		float t1 = 1 / a.mass;
		float t2 = 1 / b.mass;
		float t3 = ra.mag() * ra.mag() / a.inertia;
		float t4 = rb.mag() * rb.mag() / b.inertia;

		float j = numerator / (t1 + t2 + t3 + t4);
		Vec2 force = ci.normal * j *(1.0f/ dt);

		a.addForce(force);
		float torqueA = Cross(ra, force);
		a.addTorque(torqueA);

		b.addForce(-force);
		Vec2 vn = Dot(ci.normal, b.velocity) * ci.normal;
		Vec2 vt = b.velocity - vn;
		float torqueB = -copysign(1.0f,Cross(vt, ci.normal))*rb.mag()*force.mag();
		b.addTorque(torqueB);
	}

	void staticBoxDynamiCircleResponse(ContactInfo& ci, float dt)
	{
		BoxCollider& a = *static_cast<BoxCollider*>(ci.a);
		CircleCollider& b = *static_cast<CircleCollider*>(ci.b);
		
		if (b.isDynamic)
			b.position += ci.penetration * ci.normal;

		//Vec2 ra = ci.points[0] - a.position;
		Vec2 rb = ci.points[0] - b.position;
		//Vec2 pa = a.velocity + a.angularVelocity * Tangent(ra);
		Vec2 pb = b.velocity + b.angularVelocity * Tangent(rb);

		float vRel = Dot(ci.normal, pb );
		float numerator = (1 - epsilon) * vRel;

		//float t1 = 1 / a.mass;
		float t2 = 1 / b.mass;
		//float t3 = ra.mag() * ra.mag() / a.inertia;
		float t4 = rb.mag() * rb.mag() / b.inertia;

		float j = numerator / (t2 + t4);
		Vec2 force = ci.normal * j * (1.0f / dt);


		if (fabsf(j) > restitution)
		{
			b.addForce(-force);
			Vec2 vn = Dot(ci.normal, b.velocity) * ci.normal;
			Vec2 vt = b.velocity - vn;
			float torqueB = -copysign(1.0f, Cross(vt, ci.normal)) * rb.mag() * force.mag();
			b.addTorque(torqueB);
		}
		else
		{
			Vec2 vn = Dot(ci.normal, b.velocity) * ci.normal;
			Vec2 vt = b.velocity - vn;
			b.setVelocity(vt);
			b.setAngularVelocity( ( - copysign(1.0f, Cross(vt, ci.normal)) / b.radius)* vt.mag());
		}
	}

	bool circleCircleOverlap(const CircleCollider& a, const CircleCollider& b)
	{
		Vec2 dp = b.position - a.position;
		float length = dp.mag();
		float penetration = a.radius + b.radius - length;
		if (penetration > 0)
			return true;
		else
			return false;
	}

	void circleCircleResponse(float dt, CircleCollider& a, CircleCollider& b)
	{
		Vec2 dp = b.position - a.position;
		float penetration = dp.mag() - (a.radius + b.radius);
		penetration = fabsf(penetration);
		dp.normalize();
		a.position += dp * (-penetration * 0.5f);
		b.position += dp * (penetration * 0.5f);
		Vec2 relMomentum = a.momentum - b.momentum;


		float mag = Dot(dp, relMomentum);
		if (mag > 0)
		{
			Vec2 relMomentumNorm = mag * dp * restitution;
			b.addForce(relMomentumNorm *(1.0f/dt));
			a.addForce(relMomentumNorm * (-1.0f / dt));
		}

		Vec2 dpPerp = { -dp.y, dp.x };
		mag = Dot(dpPerp, relMomentum);
		if (mag > 0)
		{
			b.addTorque(mag*restitution * (1.0f / dt));
			a.addTorque(mag*restitution * (-1.0f / dt));
		}

	}

	bool containsPair(std::vector< ContactInfo>& contactInfos, Collider* a, Collider* b)
	{
		bool pairFound = false;
		for (int i = 0; i < contactInfos.size(); i++)
		{
			if ((contactInfos[i].a == a && contactInfos[i].b == b) || contactInfos[i].a == b && contactInfos[i].b == a)
			{
				pairFound = true;
				break;
			}
		}
		return pairFound;
	}

	void stepSquare(float dt, SquareSpace::Square& square)
	{
		std::vector<BoxCollider*> boxColliders;
		std::vector<CircleCollider*> circleColliders;

		for (std::list<Collider*>::const_iterator it = square.colliders.begin(); it != square.colliders.end(); it++)
		{
			switch ((*it)->getType())
			{
			case ColliderType::BOX:
			{
				BoxCollider* bc = dynamic_cast<BoxCollider*>(*it);
				boxColliders.push_back(bc);
				break;
			}
			case ColliderType::CIRCLE:
			{
				CircleCollider* cc = dynamic_cast<CircleCollider*>(*it);
				circleColliders.push_back(cc);
				break;
			}
			}
		}



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
						if (!containsPair(circleColliderPairs, a, b))
							circleColliderPairs.push_back(ci);
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
					//if the circles overlap, see if the pair is already found and
						//if not add it to the vector/list
					if(!containsPair(circleBoxColliderPairs, a, b))
						circleBoxColliderPairs.push_back(ci);
				}

			}
		}
	}

	void step(float dt)
	{
		circleBoxColliderPairs.clear();
		circleColliderPairs.clear();
		//clear all squares colliders
		for (int i = 0; i < squareSpace->squares.size(); i++)
		{
			SquareSpace::Square& square = squareSpace->squares[i];
			square.colliders.clear();
		}
		squareSpace->outside.colliders.clear();

		//move all object
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
		
		//insert the colliders into the squares they belong to
		for (int i = 0; i < circleColliders.size(); i++)
		{
			circleColliders[i]->setAABB();
			std::list<int> sis = squareSpace->getContainmentSquareIndices(circleColliders[i]->aabb);
			for (std::list<int>::const_iterator it = sis.begin(); it != sis.end(); it++)
			{
				if (*it == -1)
					squareSpace->outside.colliders.push_back(circleColliders[i]);
				else
					squareSpace->squares[*it].colliders.push_back(circleColliders[i]);
			}
		}
		for (int i = 0; i < boxColliders.size(); i++)
		{
			boxColliders[i]->setAABB();
			std::list<int> sis = squareSpace->getContainmentSquareIndices(boxColliders[i]->aabb);
			for (std::list<int>::const_iterator it = sis.begin(); it != sis.end(); it++)
			{
				if (*it == -1)
					squareSpace->outside.colliders.push_back(boxColliders[i]);
				else
					squareSpace->squares[*it].colliders.push_back(boxColliders[i]);
			}
		}

		//solve each of the squares
		for (int i = 0; i < squareSpace->squares.size(); i++)
		{
			SquareSpace::Square& square = squareSpace->squares[i];
			stepSquare(dt, square);
		}

		//step the outside square
		stepSquare(dt, squareSpace->outside);

		//handle collsions
				//handle the collisions
		for (int k = 0; k < circleColliderPairs.size(); k++)
		{
			CircleCollider& a = *static_cast<CircleCollider*>(circleColliderPairs[k].a);
			CircleCollider& b = *static_cast<CircleCollider*>(circleColliderPairs[k].b);
			circleCircleResponse(dt, a, b);
		}

		for (int k = 0; k < circleBoxColliderPairs.size(); k++)
		{
			BoxCollider& a = *static_cast<BoxCollider*>(circleBoxColliderPairs[k].a);
			CircleCollider& b = *static_cast<CircleCollider*>(circleBoxColliderPairs[k].b);
			if (a.isDynamic && b.isDynamic)
				boxCircleResponse(circleBoxColliderPairs[k], dt);
			else if (!a.isDynamic && b.isDynamic)
				staticBoxDynamiCircleResponse(circleBoxColliderPairs[k], dt);
		}
	}

	void stepAll(float dt)
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
			CircleCollider& a = *static_cast<CircleCollider*>(circleColliderPairs[k].a);
			CircleCollider& b = *static_cast<CircleCollider*>(circleColliderPairs[k].b);
			circleCircleResponse(dt, a, b);
		}

		for (int k = 0; k < circleBoxColliderPairs.size(); k++)
		{
			BoxCollider& a = *static_cast<BoxCollider*>(circleBoxColliderPairs[k].a);
			CircleCollider& b = *static_cast<CircleCollider*>(circleBoxColliderPairs[k].b);
			if (a.isDynamic && b.isDynamic)
				boxCircleResponse(circleBoxColliderPairs[k], dt);
			else if (!a.isDynamic && b.isDynamic)
				staticBoxDynamiCircleResponse(circleBoxColliderPairs[k], dt);
		}
	}
};