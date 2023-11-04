#ifndef MATRIX_H
#define MATRIX_H

// Biblioteki do obslugi plikow, wypisywania, pomiaru czasu, liczb pseudolosowych
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>
#include <queue>
#include <iterator>
#include <chrono>
#include <stack>

#include "BnBNode.h"

#define TIME_LIMIT 45  // sek. Maksymalny czas wykonywania algorytmu - ustalony w celu zapobiezenia nadmiernemu zuzyciu pamieci operacyjnej

class Graph
{
    public:
        Graph();
        Graph(short int N);
        Graph(std::string fname);
        Graph& operator=(const Graph& sec);
        static void printGraph(std::vector<std::vector<short int>> matrix);
        void printGraph();
        double timeBranchAndBoundATSP();
    private:
        short int size;
        std::vector<std::vector<short int>> matrix;
};

#endif // MATRIX_H
