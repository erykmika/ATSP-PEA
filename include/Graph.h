#ifndef MATRIX_H
#define MATRIX_H

// Biblioteki do obslugi plikow, wypisywania, pomiaru czasu, liczb pseudolosowych
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>
#include <queue>
#include <chrono>
#include <stack>

#include "BnBNode.h"

#define TIME_LIMIT_ON
#define TIME_LIMIT 300000  // 5 min = 300000 ms Maksymalny czas wykonywania algorytmu
//#define TIME_LIMIT 500

class Graph
{
    public:
        Graph();
        Graph(int N);
        Graph(std::string fname);
        Graph& operator=(const Graph& sec);
        void printGraph() const;
        double timeBranchAndBoundATSP() const;
        int calcUpBnd() const;
    private:
        int size;
        std::vector<std::vector<int>> matrix;
};

#endif // MATRIX_H
