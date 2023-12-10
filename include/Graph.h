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
#include <float.h>
#include <cmath>
#include <utility>

// Klasa reprezentujaca sciezke
#include "Route.h"

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
        unsigned solveSimulatedAnnealing(double delta, int timeLimit);
        unsigned solveTabuSearch(char neighbourFunction, int timeLimit);
        /*
            Metody pomocniczne wykorzystywane w algorytmie SA
        */
        double generateInitialTemp();

        // Obliczanie kosztu sciezki dla grafu
        int calculateRouteCost(Route& r) const;
        // Generowanie rozwiazania zachlannie
        Route generateInitialSolution() const;
    private:
        // Rozmiar macierzy/problemu ATSP
        int size;
        // Macierz
        std::vector<std::vector<int>> matrix;
};

#endif //GRAPH_H
