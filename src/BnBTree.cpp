#include "BnBTree.h"

BnBTree::BnBTree(int** matrix, int size)
{
    root = new BnBNode(matrix, size, size-1);
}

BnBNode* BnBTree::getRoot()
{
    return root;
}

BnBTree::~BnBTree()
{
    BnBNode::deleteRecursively(root);
}
