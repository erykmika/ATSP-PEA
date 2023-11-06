#ifndef MATRIX_H
#define MATRIX_H

// Biblioteki do obslugi plikow, wypisywania, pomiaru czasu, liczb pseudolosowych
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>
#include <chrono>
#include <vector>

#include "BnBNode.h"
#include "BnBStack.h"

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
            Metody rozwiazujace problem ATSP za pomoca branch-and-bound
        */
        double timeBranchAndBoundATSP() const;          // Bez limitu czasu
        double timeBranchAndBoundATSPlimited() const;   // Limit czasu okreslony jak wyzej

        // Obliczenie ograniczenia gornego dla dlugosci cyklu Hamiltona (zachlannie)
        int calcUpBnd() const;
    private:
        // Rozmiar macierzy/problemu ATSP
        int size;
        // Macierz
        std::vector<std::vector<int>> matrix;
};

#endif // MATRIX_H
