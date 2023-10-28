#ifndef BNBTREE_H
#define BNBTREE_H

#include "BnBNode.h"

class BnBTree
{
    public:
        BnBTree(int** matrix, int size);
        ~BnBTree();
        BnBNode* getRoot();
    private:
        BnBNode* root;
};

#endif // BNBTREE_H
