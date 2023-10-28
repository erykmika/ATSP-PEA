#include "BnBTree.h"

BnBTree::BnBTree(int** matrix, int size)
{
    this->size = size;
    bool* visitedArray = new bool[size];

    for(int i=0; i<size; i++)
        visitedArray[i] = 0;

    root = new BnBNode(0, matrix, size, size-1, visitedArray, -1, -1, 0);

    delete [] visitedArray;
}

BnBNode* BnBTree::getRoot()
{
    return root;
}

BnBTree::~BnBTree()
{
    BnBNode::cleanup(size);
    BnBNode::deleteRecursively(root);
}
