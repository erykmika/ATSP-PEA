#include "Route.h"

Route::Route():Route(5){}

Route::Route(int n)
{
    route.resize(n);
}

/*
    Metoda dla danego n generuje losowy ciag liczb ze zbioru {1,...,n}.
    Jest to permutacja - trasa komiwoja¿era bez pierwszego i ostatniego przystanku na trasie (0).
    np. 0-1-2-3-0
    trasa reprezentowana jako 1-2-3
*/
void Route::generateRandom()
{
    for(unsigned i=1; i<=route.size(); i++)
        route[i-1] = i;
    // !!!
    std::random_shuffle(route.begin(), route.end());
}

std::string Route::toString()
{
    std::string res = "";
    for(unsigned i=0; i<route.size(); i++)
        res += std::to_string(route[i]) + " ";
    return res;
}

Route& Route::operator=(const Route& sec)
{
    route = sec.route;
    return *this;
}

int& Route::operator[](unsigned i)
{
    return route[i];
}

void Route::procedure2opt(unsigned i, unsigned j)
{
    swap(i, j);
}

void Route::procedureInverse(unsigned i, unsigned j)
{
    if(i > j) std::swap(i, j);
    while(i < j) swap(i++, j--);

}

void Route::procedureInsert(unsigned i, unsigned j)
{
    if(i > j) std::swap(i, j);

    int insertedElement = route[i];

    while(i < j-1)
    {
        swap(route[i+1], route[i]);
        i++;
    }

    route[j-1] = insertedElement;
}

void Route::swap(unsigned i, unsigned j)
{
    int temp = route[i];
    route[i] = route[j];
    route[j] = temp;
}
