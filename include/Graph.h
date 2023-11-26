#ifndef GRAPH_H
#define GRAPH_H

// Biblioteki do obslugi plikow, wypisywania, pomiaru czasu, liczb pseudolosowych
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>
#include <chrono>
#include <vector>
#include <math.h>

#include "Route.h"

#define TIME_LIMIT 300000  // 5 min = 300 000 ms = Maksymalny czas wykonywania algorytmu

// Klasa reprezentujaca graf, na ktorym badamy algorytmy rozwiazujace problem ATSP
class Graph
{
    public:
        // Konstruktory: domyslny, generujacy losowy problem rozmiaru N, wczytujacy dane z pliku
        Graph();
        Graph(int N);
        Graph(std::string fname);
        // Operator przypisania
        Graph& operator=(const Graph& sec);
        // Wypisywanie grafu/macierzy
        void printGraph() const;

        /*
            Metody rozwiazujace problem ATSP
        */
        double timeSimulatedAnnealing(double delta, int numOfIterations=2000000);

        int calculateRouteCost(Route& r) const;
        Route generateInitialSolution() const;
    private:
        // Rozmiar macierzy/problemu ATSP
        int size;
        // Macierz
        std::vector<std::vector<int>> matrix;
};

#endif //GRAPH_H
