#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

// Importujemy te same biblioteki, co w main.cpp do obslugi pseudolosowosci

#include <random>
#include <time.h>


class Graph
{
    public:
        Graph();
        Graph(int N);
        Graph(std::string fname);
        ~Graph();
        Graph& operator=(const Graph& sec);
        void printGraph();
        void bruteForceTSP();

    private:
        int siz;
        int** matrix;
};

#endif // MATRIX_H
