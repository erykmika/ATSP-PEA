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
        Route();
        Route(int n);
        Route(const std::vector<int>& r); // Konstruktor do testow
        void randomize();
        std::string toString() const;
        unsigned getSize() const;
        Route& operator=(const Route& sec);
        int& operator[](unsigned i);
        bool operator==(const Route& sec) const;

        // Operatory mutacji
        void mutateInverse(unsigned i, unsigned j);
        void mutateScramble(unsigned k);

        // Operatory krzyzowania
        Route crossoverPMX(const Route& sec);

        // obsolete
        //void procedureSwap(unsigned i, unsigned j);
        //void procedureInsert(unsigned i, unsigned j);
    private:
        std::vector<int> route;
        void swap(unsigned i, unsigned j);
};

#endif // ROUTE_H
