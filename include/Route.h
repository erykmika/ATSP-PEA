#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <algorithm>
#include <string>

class Route
{
    public:
        Route();
        Route(int n);
        void generateRandom();
        std::string toString();
        Route& operator=(const Route& sec);
        int& operator[](unsigned i);
        void procedure2opt(unsigned i, unsigned j);
    private:
        std::vector<int> route;
};

#endif // ROUTE_H
