#ifndef BNBNODE_H
#define BNBNODE_H

#include <iostream>
#include <vector>
#include <algorithm>

class BnBNode
{
    public:
        BnBNode(int nod, const std::vector<std::vector<int>>& mtx, int exRow, int prevCost,
                int pathLen, std::vector<int>& prevPath);
        std::vector<std::vector<int>>& getMatrix();
        std::vector<int>& getPath();
        int getCost() const;
        int getNode() const;
        int getNumOfVisited() const;
        bool isLeaf() const;
        int bound() const;
    private:
        std::vector<std::vector<int>> matrix;
        std::vector<int> path;
        int reduceMatrix();
        int size;
        int cost;
        int node;
        int numOfVisited;

};

#endif // BNBNODE_H
