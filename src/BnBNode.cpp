#include "BnBNode.h"

// Konstruktor wierzcholka drzewa algorytmu branch-and-bound
BnBNode::BnBNode(int nod, const std::vector<std::vector<int>>& mtx, int exRow,
                 int prevCost, int pathLen, std::vector<int>& prevPath)
{
    node = nod;
    size = mtx.size();
    matrix = mtx;

    path = prevPath;
    path.push_back(node);

    // Wykluczamy wiersz i kolumne
    if(exRow>=0)
    {
        for(int i=0; i<size; i++)
        {
            matrix[exRow][i] = -1;
            matrix[i][node] = -1;
        }
    }
    matrix[nod][0] = -1;

    numOfVisited = pathLen+1;

    cost = reduceMatrix() + prevCost;
}

// Zwrocenie referencji do macierzy wierzcholka
std::vector<std::vector<int>>& BnBNode::getMatrix()
{
    return matrix;
}

// Metoda redukujaca macierz - obliczenie ograniczenia dolnego
int BnBNode::reduceMatrix()
{
    int sumOfReduction = 0;

    // Petla do przechodzenia jednoczesnie po wierszach i kolumnach macierzy wierzcholka
    for(int i=0; i<size; i++)
    {
        int rowMin = INT_MAX;
        int colMin = INT_MAX;
        // Znajdujemy minimum w wierszu i kolumnie, pomijamy ujemne wartosci (np. z przekatnej).
        for(int j=0; j<size; j++)
        {
            if(matrix[i][j] >= 0 && matrix[i][j] < rowMin) rowMin = matrix[i][j];
            if(matrix[j][i] >= 0 && matrix[j][i] < colMin) colMin = matrix[j][i];
        }
        // Mozliwe, ze 0 - wczesniej pozostalo MAX
        rowMin = ((rowMin!=INT_MAX) ? rowMin : 0);
        colMin = ((colMin!=INT_MAX) ? colMin : 0);
        // Koszt - suma redukcji wierszy row i kolumn col
        sumOfReduction += rowMin + colMin;

        // Redukujemy macierz
        for(int j=0; j<size; j++)
        {
            matrix[i][j] -= rowMin;
            matrix[j][i] -= colMin;
        }
    }
    return sumOfReduction;
}

// Gettery - zwracaja kolejno koszt wierzcholka, indeks (numer), liczbe odwiedzonych dotychczas wierzcholkow

int BnBNode::getCost() const
{
    return cost;
}

int BnBNode::getNode() const
{
    return node;
}

int BnBNode::getNumOfVisited() const
{
    return numOfVisited;
}

// Czy wierzcholek jest lisciem
bool BnBNode::isLeaf() const
{
    return numOfVisited==size;
}

// Metoda zwracajaca sciezke dla danego wierzcholka
std::vector<int>& BnBNode::getPath()
{
    return path;
}
