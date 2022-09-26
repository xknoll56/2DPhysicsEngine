#pragma once

// The objects that we want stored in the quadtree
struct QuadNode {
    vec2 pos;
    int data;
    QuadNode(vec2 _pos, int _data)
    {
        pos = _pos;
        data = _data;
    }
    QuadNode() { data = 0; }
};

// The main quadtree class
class QuadTree {
    // Hold details of the boundary of this node
    vec2 topLeft;
    vec2 botRight;

    // Contains details of node
    QuadNode* n;

    // Children of this tree
    QuadTree* topLeftTree;
    QuadTree* topRightTree;
    QuadTree* botLeftTree;
    QuadTree* botRightTree;

public:
    QuadTree()
    {
        topLeft = vec2(0, 0);
        botRight = vec2(0, 0);
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
        topLeft = topL;
        botRight = botR;
    }
    void insert(QuadNode*);
    QuadNode* search(vec2);
    bool inBoundary(vec2);
};

// Insert a node into the quadtree
void QuadTree::insert(QuadNode* node)
{
    if (node == NULL)
        return;

    // Current quad cannot contain it
    if (!inBoundary(node->pos))
        return;

    // We are at a quad of unit area
    // We cannot subdivide this quad further
    if (abs(topLeft.x - botRight.x) <= 1
        && abs(topLeft.y - botRight.y) <= 1) {
        if (n == NULL)
            n = node;
        return;
    }

    if ((topLeft.x + botRight.x) / 2 >= node->pos.x) {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
            if (topLeftTree == NULL)
                topLeftTree = new QuadTree(
                    vec2(topLeft.x, topLeft.y),
                    vec2((topLeft.x + botRight.x) / 2,
                        (topLeft.y + botRight.y) / 2));
            topLeftTree->insert(node);
        }

        // Indicates botLeftTree
        else {
            if (botLeftTree == NULL)
                botLeftTree = new QuadTree(
                    vec2(topLeft.x,
                        (topLeft.y + botRight.y) / 2),
                    vec2((topLeft.x + botRight.x) / 2,
                        botRight.y));
            botLeftTree->insert(node);
        }
    }
    else {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
            if (topRightTree == NULL)
                topRightTree = new QuadTree(
                    vec2((topLeft.x + botRight.x) / 2,
                        topLeft.y),
                    vec2(botRight.x,
                        (topLeft.y + botRight.y) / 2));
            topRightTree->insert(node);
        }

        // Indicates botRightTree
        else {
            if (botRightTree == NULL)
                botRightTree = new QuadTree(
                    vec2((topLeft.x + botRight.x) / 2,
                        (topLeft.y + botRight.y) / 2),
                    vec2(botRight.x, botRight.y));
            botRightTree->insert(node);
        }
    }
}

// Find a node in a quadtree
QuadNode* QuadTree::search(vec2 p)
{
    // Current quad cannot contain it
    if (!inBoundary(p))
        return NULL;

    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (n != NULL)
        return n;

    if ((topLeft.x + botRight.x) / 2 >= p.x) {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topLeftTree == NULL)
                return NULL;
            return topLeftTree->search(p);
        }

        // Indicates botLeftTree
        else {
            if (botLeftTree == NULL)
                return NULL;
            return botLeftTree->search(p);
        }
    }
    else {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topRightTree == NULL)
                return NULL;
            return topRightTree->search(p);
        }

        // Indicates botRightTree
        else {
            if (botRightTree == NULL)
                return NULL;
            return botRightTree->search(p);
        }
    }
};

// Check if current quadtree contains the point
bool QuadTree::inBoundary(vec2 p)
{
    return (p.x >= topLeft.x && p.x <= botRight.x
        && p.y >= topLeft.y && p.y <= botRight.y);
}