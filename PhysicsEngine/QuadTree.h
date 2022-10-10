#pragma once

// The objects that we want stored in the quadtree
// may be also good to store a collider...
struct QuadNode {
    RigidBody* rb;
    QuadNode(RigidBody* _rb)
    {
        rb = _rb;
    }
    QuadNode() {  }
};

// The main quadtree class
class QuadTree {
    // Hold details of the boundary of this node
    vec2 botLeft;
    vec2 topRIght;

    // Contains details of node
    QuadNode* n;

public:
    // Children of this tree
    QuadTree* topLeftTree;
    QuadTree* topRightTree;
    QuadTree* botLeftTree;
    QuadTree* botRightTree;

    QuadTree()
    {
        botLeft = vec2(0, 0);
        topRIght = vec2(0, 0);
        n = NULL;
        topLeftTree = NULL;
        topRightTree = NULL;
        botLeftTree = NULL;
        botRightTree = NULL;
    }
    QuadTree(vec2 topL, vec2 botR)
    {
        n = NULL;
        topLeftTree = NULL;
        topRightTree = NULL;
        botLeftTree = NULL;
        botRightTree = NULL;
        botLeft = topL;
        topRIght = botR;
    }

    vec2 midPoint;
    vec2 halfExtents;
    void calculateDrawInfo();
    void insert(QuadNode*);
    QuadNode* search(RigidBody* rb);
    bool inBoundary(RigidBody* rb);
};

void QuadTree::calculateDrawInfo()
{
    midPoint = (botLeft + topRIght) * 0.5f;
    halfExtents = { (topRIght.x - botLeft.x) * 0.5f, (botLeft.y - topRIght.y) * 0.5f };
}

// Insert a node into the quadtree
void QuadTree::insert(QuadNode* node)
{
    if (node == NULL)
        return;

    // Current quad cannot contain it
    if (!inBoundary(node->rb))
        return;

    // We are at a quad of unit area
    // We cannot subdivide this quad further
    if (abs(botLeft.x - topRIght.x) <= 1
        && abs(botLeft.y - topRIght.y) <= 1) {
        if (n == NULL)
            n = node;
        return;
    }

    if ((botLeft.x + topRIght.x) / 2 >= node->rb->position.x) {
        // Indicates topLeftTree
        if ((botLeft.y + topRIght.y) / 2 >= node->rb->position.y) {
            if (topLeftTree == NULL)
                topLeftTree = new QuadTree(
                    vec2(botLeft.x, botLeft.y),
                    vec2((botLeft.x + topRIght.x) / 2,
                        (botLeft.y + topRIght.y) / 2));
            topLeftTree->insert(node);
        }

        // Indicates botLeftTree
        else {
            if (botLeftTree == NULL)
                botLeftTree = new QuadTree(
                    vec2(botLeft.x,
                        (botLeft.y + topRIght.y) / 2),
                    vec2((botLeft.x + topRIght.x) / 2,
                        topRIght.y));
            botLeftTree->insert(node);
        }
    }
    else {
        // Indicates topRightTree
        if ((botLeft.y + topRIght.y) / 2 >= node->rb->position.y) {
            if (topRightTree == NULL)
                topRightTree = new QuadTree(
                    vec2((botLeft.x + topRIght.x) / 2,
                        botLeft.y),
                    vec2(topRIght.x,
                        (botLeft.y + topRIght.y) / 2));
            topRightTree->insert(node);
        }

        // Indicates botRightTree
        else {
            if (botRightTree == NULL)
                botRightTree = new QuadTree(
                    vec2((botLeft.x + topRIght.x) / 2,
                        (botLeft.y + topRIght.y) / 2),
                    vec2(topRIght.x, topRIght.y));
            botRightTree->insert(node);
        }
    }
}

// Find a node in a quadtree
QuadNode* QuadTree::search(RigidBody* rb)
{
    // Current quad cannot contain it
    if (!inBoundary(rb))
        return NULL;

    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (n != NULL)
        return n;

    if ((botLeft.x + topRIght.x) / 2 >= rb->position.x) {
        // Indicates topLeftTree
        if ((botLeft.y + topRIght.y) / 2 >= rb->position.y) {
            if (topLeftTree == NULL)
                return NULL;
            return topLeftTree->search(rb);
        }

        // Indicates botLeftTree
        else {
            if (botLeftTree == NULL)
                return NULL;
            return botLeftTree->search(rb);
        }
    }
    else {
        // Indicates topRightTree
        if ((botLeft.y + topRIght.y) / 2 >= rb->position.y) {
            if (topRightTree == NULL)
                return NULL;
            return topRightTree->search(rb);
        }

        // Indicates botRightTree
        else {
            if (botRightTree == NULL)
                return NULL;
            return botRightTree->search(rb);
        }
    }
};

// Check if current quadtree contains the point
bool QuadTree::inBoundary(RigidBody* rb)
{
    return (rb->position.x >= botLeft.x && rb->position.x <= topRIght.x
        && rb->position.y <= botLeft.y && rb->position.y >= topRIght.y);
}