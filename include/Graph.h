#ifndef MATRIX_H
#define MATRIX_H

// Biblioteki do obslugi plikow, wypisywania, pomiaru czasu, liczb pseudolosowych
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>

#include "BnBNode.h"
#include "BnBTree.h"


class Graph
{
    public:
        Graph();
        Graph(int N);
        Graph(std::string fname);
        ~Graph();
        Graph& operator=(const Graph& sec);
        static void printGraph(int** matrix, int n);
        void branchAndBoundATSP();
        //double timeBruteForceATSP();

    private:
        int size;
        int** matrix;
};

#endif // MATRIX_H
