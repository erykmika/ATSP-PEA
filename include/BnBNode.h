#ifndef BNBNODE_H
#define BNBNODE_H

#include <iostream>

class BnBNode
{
    public:
        BnBNode(int** mtx, int n, int child);
        ~BnBNode();
        int reduceMatrix();
        int** getMatrix();
        BnBNode** getChildren();
        int getCost();
        void addCost(int c);
        static void deleteRecursively(BnBNode* node);
    private:
        int** matrix;
        int size;
        int cost;
        int numOfChildren;
        BnBNode** children;
        int* visited;
};

#endif // BNBNODE_H
