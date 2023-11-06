#ifndef BNBNODE_H
#define BNBNODE_H

#include <iostream>
#include <vector>

class BnBNode
{
    public:
        // Konstruktor
        BnBNode(int nod, const std::vector<std::vector<int>>& mtx, int exRow, int prevCost, std::vector<int>& prevPath);
        // Zwrocenie referencji do macierzy wierzcholka
        std::vector<std::vector<int>>& getMatrix();
        // Zwrocenie referencji do sciezki wierzcholka
        std::vector<int>& getPath();
        // Gettery (koszt, indeks wierzcholka, liczba odwiedzonych, wskaznik na nastepny)
        int getCost() const;
        int getNode() const;
        int getNumOfVisited() const;
        BnBNode* getNext() const;
        // Czy wierzcholek jest lisciem
        bool isLeaf() const;
        // Ustaw nastepny wierzcholek na stosie
        void setNext(BnBNode* nxt);
    private:
        // Macierz dla konkretnego wierzcholka drzewa rozwiazan metoda branch-and-bound
        std::vector<std::vector<int>> matrix;
        // Sciezka - czesciowe/calkowite rozwiazanie dla danego wierzcholka
        std::vector<int> path;
        // Metoda redukujaca macierz - obliczenie ograniczenia dolnego
        int reduceMatrix();
        // Rozmiar problemu
        int size;
        // Koszt przypisany do konkretnego wierzcholka
        int cost;
        // Indeks wierzcholka, ktory jest reprezentowany
        int node;
        // Wskaznik na nastepny wierzcholek na stosie
        BnBNode* next;
};

#endif // BNBNODE_H
