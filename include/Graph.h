#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iomanip>

// Importujemy te same biblioteki, co w main.cpp do obslugi pseudolosowosci

#include <random>
#include <time.h>

#include "PermutationArray.h"


class Graph
{
    public:
        Graph();
        Graph(int N);
        Graph(std::string fname);
        ~Graph();
        Graph& operator=(const Graph& sec);
        void printGraph();
        double measureBruteForceATSP();

    private:
        int size;
        int** matrix;
};

#endif // MATRIX_H
