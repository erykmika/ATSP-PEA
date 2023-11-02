#include "BnBNode.h"

BnBNode::BnBNode(short int nod, const std::vector<std::vector<short int>>& mtx, short int exRow,
                 short int prevCost, short int pathLen, std::vector<short int>& prevPath)
{
    node = nod;
    size = mtx.size();
    matrix = mtx;

    path = prevPath;
    path.push_back(node);

    // Wykluczamy wiersz i kolumne
    if(exRow>=0)
    {
        for(short int i=0; i<size; i++)
        {
            matrix[exRow][i] = -1;
            matrix[i][node] = -1;
        }
    }
    matrix[nod][0] = -1;

    numOfVisited = pathLen+1;

    cost = reduceMatrix() + prevCost;
}

std::vector<std::vector<short int>>& BnBNode::getMatrix()
{
    return matrix;
}

short int BnBNode::reduceMatrix()
{
    short int sumOfReduction = 0;

    for(short int i=0; i<size; i++)
    {
        short int rowMin = SHRT_MAX;
        short int colMin = SHRT_MAX;
        // Znajdujemy minimum w wierszu i kolumnie, pomijamy ujemne wartosci (np. z przekatnej).
        for(short int j=0; j<size; j++)
        {
            if(matrix[i][j] >= 0 && matrix[i][j] < rowMin) rowMin = matrix[i][j];
            if(matrix[j][i] >= 0 && matrix[j][i] < colMin) colMin = matrix[j][i];
        }

        rowMin = ((rowMin>=0 && rowMin!=SHRT_MAX) ? rowMin : 0);
        colMin = ((colMin>=0 && colMin!=SHRT_MAX) ? colMin : 0);
        sumOfReduction += rowMin + colMin;

        for(short int j=0; j<size; j++)
        {
            matrix[i][j] -= rowMin;
            matrix[j][i] -= colMin;
        }
    }
    return sumOfReduction;
}

short int BnBNode::getCost() const
{
    return cost;
}

short int BnBNode::getNode() const
{
    return node;
}

short int BnBNode::getNumOfVisited() const
{
    return numOfVisited;
}

bool BnBNode::isLeaf() const
{
    return numOfVisited==size;
}

std::vector<short int>& BnBNode::getPath()
{
    return path;
}
