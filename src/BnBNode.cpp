#include "BnBNode.h"

int** BnBNode::originMatrix = nullptr;

BnBNode::BnBNode(int nod, int** mtx, int n, int child, bool* visit,
                 int exRow, int exCol, int prevCost)
{
    node = nod;
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

    children = new BnBNode*[numOfChildren];
    for(int i=0; i<numOfChildren; i++)
    {
        children[i] = NULL;
    }

    visited = new bool[n];

    isLeaf = true;

    for(int i=0; i<n; i++)
    {
        visited[i] = visit[i];
        isLeaf = isLeaf && visit[i];
    }

    visited[nod] = true;

    cost = reduceMatrix() + prevCost;
    //std::cout<<"C = "<<cost<<"\n";
    if(node==0)
    {
        initializeOrigin(size);
        for(int i=0; i<size; i++)
        {
            for(int j=0; j<size; j++)
            {
                originMatrix[i][j] = matrix[i][j];
            }
        }
    }
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

void BnBNode::generateChildren()
{
    if(isLeaf) return;

    int childIndex = 0;

    int i = 0;

    while(childIndex<numOfChildren)
    {
        if(visited[i]==0)
        {
            children[childIndex] = new BnBNode(i, matrix, size, numOfChildren-1, visited, node, i, cost + originMatrix[node][i]);
            childIndex++;
        }

        i++;
    }
}

int BnBNode::getCost()
{
    return cost;
}

int BnBNode::getEdge(int i, int j)
{
    return matrix[i][j];
}

BnBNode** BnBNode::getChildren()
{
    return children;
}

void BnBNode::initializeOrigin(int size)
{
    originMatrix = new int*[size];
    for (int i = 0; i < size; i++)
    {
        originMatrix[i] = new int[size];
    }
}

void BnBNode::cleanup(int size)
{
    for (int i = 0; i < size; ++i)
    {
        delete[] originMatrix[i];
    }
    delete[] originMatrix;
}
