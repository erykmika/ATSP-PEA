#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <iostream>
#include <fstream>

// Importujemy te same biblioteki, co w main.cpp do obslugi pseudolosowosci

#include <random>
#include <time.h>


class Graph
{
    public:
        Graph(int N);
        Graph(std::string fname);
        ~Graph();
        void printGraph();

    private:
        int siz;
        int** matrix;
};

#endif // MATRIX_H
