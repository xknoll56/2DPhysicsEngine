#pragma once

class TestScene : Scene
{
public: 
	CircleCollider c1;
	CircleCollider c2;
	std::vector<CircleCollider> colliders;
	QuadTree* tree;

	TestScene(Renderer* renderer)
	{
		this->renderer = renderer;
	}
	void setup()
	{
		tree = new QuadTree({ -10,10 }, { 10, -10 });
		for (int i = 0; i < 20; i++)
		{
			CircleCollider collider;
			//collider.setVelocity(vec2{ (float)(rand() % 4)-2, (float)(rand() % 4)-2 });
			collider.position = (vec2{ (float)(rand() % 20)-10, (float)(rand() % 20)-10 });
			QuadNode n((RigidBody*)&collider);
			tree->insert(&n);
			colliders.push_back(collider);
		}

		for (int i = 0; i < colliders.size(); i++)
		{
			world.circleColliders.push_back(&colliders[i]);
		}
	}

	void drawQuadTree(QuadTree* tree)
	{
		tree->calculateDrawInfo();
		renderer->DrawBox(tree->midPoint, tree->halfExtents, 0.0f,false, { 0,1,0,1 });

		if (tree->botLeftTree != NULL)
		{
			drawQuadTree(tree->botLeftTree);
		}


		if (tree->botRightTree != NULL)
		{
			drawQuadTree(tree->botRightTree);
		}


		if (tree->topLeftTree != NULL)
		{
			drawQuadTree(tree->topLeftTree);
		}


		if (tree->topRightTree != NULL)
		{
			drawQuadTree(tree->topRightTree);
		}
	}

	void update(float dt)
	{
		moveCamera(5.0f, dt);
		world.stepNoSpaceDivision(dt);
		for (int i = 0; i < colliders.size(); i++)
		{
			renderer->DrawCircleCollider(&colliders[i], { 1,1,1,1 });
		}

		drawQuadTree(tree);
	}
};