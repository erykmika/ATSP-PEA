#ifndef MATRIX_H
#define MATRIX_H

// Biblioteki do obslugi plikow, wypisywania, pomiaru czasu, liczb pseudolosowych
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>

// Wlasna biblioteka umozliwiajaca generowanie permutacji do badania problemu ATSP
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
        double timeBruteForceATSP();

    private:
        int size;
        int** matrix;
};

#endif // MATRIX_H
