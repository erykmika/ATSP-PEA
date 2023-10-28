#include "BnBNode.h"

BnBNode::BnBNode(int** mtx, int n, int child)
{
    size = n;
    numOfChildren = child;
    matrix = new int*[size];

    for(int i=0; i<size; i++)
    {
        matrix[i] = new int [size];
        for(int j=0; j<size; j++)
        {
            matrix[i][j] = mtx[i][j];
        }
    }

    children = new BnBNode*[numOfChildren];
    for(int i=0; i<numOfChildren; i++)
    {
        children[i] = NULL;
    }

    visited = new int[n];

    cost += reduceMatrix();
}

BnBNode::~BnBNode()
{
    for(int i=0; i<size; i++)
    {
        delete [] matrix[i];
    }
    delete [] matrix;
    delete [] children;
    delete [] visited;
}

int** BnBNode::getMatrix()
{
    return matrix;
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
        // Dodajemy wartosc redukcji do sumy. Pomijamy, jezeli ujemna (np. caly wiersz wykluczony)
        sumOfReduction += (rowMin >= 0) ? rowMin : 0;
        // Redukujemy wiersz o minimum
        for(int j=0; j<size; j++)
        {
            matrix[i][j] -= rowMin;
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
        sumOfReduction += (colMin >= 0) ? colMin : 0;
        // Redukujemy wiersz o minimum
        for(int i=0; i<size; i++)
        {
            matrix[i][j] -= colMin;
        }
    }
    std::cout << "L = " << sumOfReduction << "\n";
    return sumOfReduction;
}

void BnBNode::deleteRecursively(BnBNode* node)
{
    if(node == NULL) return;

    // Przechodzimy po wszystkich potomkach wezla i usuwamy je rekurencyjnie

    for(int i=0; i<node->numOfChildren; i++)
    {
        if(node->children[i]!=NULL) deleteRecursively(node->children[i]);
    }

    delete node;
}

void BnBNode::addCost(int c)
{
    cost += c;
}

int BnBNode::getCost()
{
    return cost;
}

BnBNode** BnBNode::getChildren()
{
    return children;
}
