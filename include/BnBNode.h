#ifndef BNBNODE_H
#define BNBNODE_H

#include <iostream>
#include <vector>



class BnBNode
{
    public:
        BnBNode(int nod, int** mtx, int n, int exRow, int exCol, int prevCost, int pathLen, std::vector<int>& prevPath);
        ~BnBNode();
        int** getMatrix();
        int getCost() const;
        int getNode();
        bool isLeaf();
        std::vector<int> path;
        int numOfVisited;
        bool operator<(const BnBNode& sec) const;
    private:
        int reduceMatrix();
        int** matrix;
        int size;
        int cost;
        int node;
};

struct CmpCost
{
    bool operator()(const BnBNode* lhs, const BnBNode* rhs) const
    {
        return lhs->getCost() > rhs->getCost();
    }
};

#endif // BNBNODE_H
