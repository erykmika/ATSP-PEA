#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <iostream>

// Klasa reprezentujaca trase komiwojazera - permutacja ciagu 1,..,n
class Route
{
    public:
        // Konstruktory
        Route();
        Route(int n);
        Route(const std::vector<int>& r); // Konstruktor do testow
        Route(const Route& sec);
        // Metoda do przestawiania elementow trasy w sposob losowy
        void randomize();
        std::string toString() const;

        // Operatory przypisania, dostepu do elementow, porownania
        Route& operator=(const Route& sec);
        int& operator[](unsigned i);
        bool operator==(const Route& sec) const;
        bool operator<(const Route& sec) const;

        // Operatory mutacji
        void mutateInverse(unsigned i, unsigned j);
        void mutateScramble(unsigned k);

        // Operatory krzyzowania
        Route crossoverPMX(const Route& sec);

        // Operator uzyty do tworzenia populacji poczatkowej
        void procedureSwap(unsigned i, unsigned j);

        // Getter/setter - koszt sciezki
        unsigned getCost() const;
        void setCost(unsigned cost);
    private:
        std::vector<int> route;
        void swap(unsigned i, unsigned j);
        unsigned cost;
};
#endif // ROUTE_H
