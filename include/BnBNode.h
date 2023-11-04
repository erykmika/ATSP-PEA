#ifndef BNBNODE_H
#define BNBNODE_H

#include <iostream>
#include <vector>
#include <algorithm>

class BnBNode
{
    public:
        BnBNode(short int nod, const std::vector<std::vector<short int>>& mtx, short int exRow, short int prevCost,
                short int pathLen, std::vector<short int>& prevPath);
        std::vector<std::vector<short int>>& getMatrix();
        std::vector<short int>& getPath();
        short int getCost() const;
        short int getNode() const;
        short int getNumOfVisited() const;
        bool isLeaf() const;
        short int bound() const;
    private:
        std::vector<std::vector<short int>> matrix;
        std::vector<short int> path;
        short int reduceMatrix();
        short int size;
        short int cost;
        short int node;
        short int numOfVisited;

};

struct CmpCost
{
    bool operator()(const BnBNode* left, const BnBNode* right) const
    {
        return left->getCost() > right->getCost();
    }
};

#endif // BNBNODE_H
