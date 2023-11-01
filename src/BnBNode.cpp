#include "BnBNode.h"

BnBNode::BnBNode(int nod, int** mtx, int n, int exRow, int exCol,
                 int prevCost, int pathLen, std::vector<int>& prevPath)
{
    node = nod;
    size = n;
    matrix = new int*[size];

    path = prevPath;
    path.push_back(node);

    for(int i=0; i<size; i++)
    {
        matrix[i] = new int [size];
        for(int j=0; j<size; j++)
        {
            matrix[i][j] = mtx[i][j];
        }
    }

    // Wykluczamy wiersz i kolumne
    if(exRow>=0 && exCol>=0)
    {
        for(int j=0; j<size; j++)
        {
            matrix[exRow][j] = -1;
        }

        for(int i=0; i<size; i++)
        {
            matrix[i][exCol] = -1;
        }
    }
    matrix[nod][0] = -1;

    numOfVisited = pathLen+1;

    cost = reduceMatrix() + prevCost;
}

BnBNode::~BnBNode()
{
    for(int i=0; i<size; i++)
    {
        delete [] matrix[i];
    }
    delete [] matrix;
}

int** BnBNode::getMatrix()
{
    return matrix;
}

bool BnBNode::isLeaf()
{
    return numOfVisited==size;
}

int BnBNode::reduceMatrix()
{
    int sumOfReduction = 0;

    // Iterujemy po wierszach
    for(int i=0; i<size; i++)
    {
        int rowMin = INT_MAX;
        // Znajdujemy minimum w wierszu, pomijamy ujemne wartosci (np. z przekatnej).
        for(int j=0; j<size; j++)
        {
            if(matrix[i][j] >= 0 && matrix[i][j] < rowMin) rowMin = matrix[i][j];
        }

        if(rowMin >= 0 && rowMin!=INT_MAX)
        {
            // Dodajemy wartosc redukcji do sumy. Pomijamy, jezeli ujemna (np. caly wiersz wykluczony)
            sumOfReduction += rowMin;
            // Redukujemy wiersz o minimum
            for(int j=0; j<size; j++)
            {
                matrix[i][j] -= rowMin;
            }
        }
    }

    // Iterujemy po kolumnach
    for(int j=0; j<size; j++)
    {
        int colMin = INT_MAX;
        // Znajdujemy minimum w wierszu, pomijamy ujemne wartosci (np. z przekatnej).
        for(int i=0; i<size; i++)
        {
            if(matrix[i][j] >= 0 && matrix[i][j] < colMin) colMin = matrix[i][j];
        }
        if(colMin >= 0 && colMin !=INT_MAX)
        {
            // Dodajemy wartosc redukcji do sumy. Pomijamy, jezeli ujemna (np. cala kolumna wykluczona)
            sumOfReduction += colMin;
            // Redukujemy kolumne o minimum
            for(int i=0; i<size; i++)
            {
                matrix[i][j] -= colMin;
            }
        }
    }

    return sumOfReduction;
}

int BnBNode::getCost() const
{
    return cost;
}

int BnBNode::getNode()
{
    return node;
}

bool BnBNode::operator<(const BnBNode& sec) const
{
    return cost > sec.cost;
}
