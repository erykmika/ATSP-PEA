#ifndef BNBNODE_H
#define BNBNODE_H

#include <iostream>

class BnBNode
{
    public:
        BnBNode(int nod, int** mtx, int n, int child, bool* visit,
                int exRow, int exCol, int prevCost);
        ~BnBNode();
        int** getMatrix();
        BnBNode** getChildren();
        int getCost();
        int getEdge(int i, int j);
        void addCost(int c);
        void generateChildren();
        static void deleteRecursively(BnBNode* node);
        static void initializeOrigin(int size);
        static void cleanup(int size);
    private:
        int reduceMatrix();
        int** matrix;
        static int** originMatrix;
        int size;
        int cost;
        int node;
        bool isLeaf;
        int numOfChildren;
        BnBNode** children;
        bool* visited;
        BnBNode* root;
};

#endif // BNBNODE_H
