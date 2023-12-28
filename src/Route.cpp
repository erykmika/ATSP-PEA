#include "Route.h"

Route::Route():Route(5) {}

Route::Route(int n)
{
    route.resize(n);
    for(int i=1; i<=n; i++)
    {
        route[i-1] = i;
    }
}

// Konstruktor do testow metod
Route::Route(const std::vector<int>& r) {
    route = r;
}

/*
    Metoda dla danego n generuje losowy ciag liczb ze zbioru {1,...,n}.
    Jest to permutacja - trasa komiwojazera bez pierwszego i ostatniego przystanku na trasie (0).
    np. 0-1-2-3-0
    trasa reprezentowana jako 1-2-3
*/
void Route::randomize()
{
    std::random_shuffle(route.begin(), route.end());
}

std::string Route::toString() const
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

bool Route::operator==(const Route& sec) const
{
    return route==sec.route;
}

unsigned Route::getSize() const
{
    return route.size();
}

/*
// Metoda wykonujaca operacje zamiany miejscami dwoch wierzcholkow na trasie.
void Route::procedureSwap(unsigned i, unsigned j)
{
    swap(i, j);
}
*/

// Mutacja typu inverse - iteracyjne odwracanie kolejnosci elementow
void Route::mutateInverse(unsigned i, unsigned j)
{
    if(i > j)
    {
        int temp = i;
        i = j;
        j = temp;
    }

    while(i < j) swap(i++, j--);

}

// Mutacja typu scramble - losowe przestawianie k elementow trasy
void Route::mutateScramble(unsigned k)
{
    if(k > route.size()) return;
    // Kopia obecnej trasy sluzaca do oznaczania wybranych elementow/indeksow - jako 0
    Route markedAsVisited = *this;
    // Wybrane indeksy do przestawiania
    std::vector<int> chosenIndices = {};

    // Wybierane sa k elementy trasy, ktore beda losowo przestawiane
    while(chosenIndices.size() < k)
    {
        int chosenIndex = rand() % ( route.size() );
        if( markedAsVisited[chosenIndex] != 0 )
        {
            chosenIndices.push_back(chosenIndex);
            markedAsVisited[chosenIndex] = 0;
        }
    }

    // Wczesniej wybrane elementy trasy sa losowo przestawiane - wywolanie wlasnej metody pomocniczej
    for(unsigned i=0; i<chosenIndices.size(); i++)
        swap( chosenIndices[i], chosenIndices[ rand() % chosenIndices.size()] );
}


Route Route::crossoverPMX(const Route& sec)
{
    // Warunek oczywisty - nie rozwazamy tras o roznej dlugosci
    //if(route.size() != sec.route.size()) return Route(route.size());
    // Losowe wybranie segmentu do krzyzowania
    unsigned routeSize = route.size();
    unsigned a = rand() % routeSize;
    unsigned b = rand() % routeSize;
    if(a == b) b = (b + 1) % routeSize;

    // Zamieniamy jezeli taka koniecznosc, aby a <= b
    if(a > b) {
        unsigned temp = a;
        a = b;
        b = temp;
    }

    //a = 3;
    //b = 6;
    std::cout<<"a = "<<a<<" b = "<<b<<"\n";
    // Wynikowy potomek
    Route offspring = sec;
    // Skopiowanie wybranego segmentu z pierwszego rodzica (*this)
    for(unsigned i=a; i<=b; i++)
    {
        offspring[i] = this->route[i];
    }

    std::vector<std::pair<int, int>> pairs = {};

    // Wyznaczenie par (i, j); elementy, ktore nie zostaly skopiowane w analogicznym segmencie z drugiego rodzica
    for(unsigned i=a; i<=b; i++)
    {
        int secRouteElement = sec.route[i];
        bool isFound = false;
        for(unsigned k=a; k<=b; k++) {
            if(secRouteElement == this->route[k]) {
                isFound = true;
                break;
            }
        }
        if(isFound) continue;
        pairs.push_back(std::pair<int, int>(secRouteElement, this->route[i]));
    }

    // Umieszczanie elementow z par (i, j)
    for(unsigned p=0; p<pairs.size(); p++)
    {
        int i = pairs[p].first;
        int j = pairs[p].second;
        int destIndex = -1;

        unsigned it = 0;
        while( it < routeSize) {
            // Czy wewnatrz skopiowanego fragmentu
            bool withinCopiedSegment = ( it >= a && it <= b );
            if( sec.route[it] == j && withinCopiedSegment )
            {
                j = this->route[it];
                it = 0;
                continue;
            } else if ( sec.route[it] == j ){
                destIndex = it;
                break;
            }
            it++;
        }
        // Umieszczenie i na pozycji zajmowanej przez j
        offspring[destIndex] = i;
    }
    return offspring;
}

/*
// Wstawianie elementu na trasie miedzy dwoma indeksami
void Route::procedureInsert(unsigned i, unsigned j)
{
    if(i > j)
    {
        int temp = i;
        i = j;
        j = temp;
    }

    int insertedElement = route[i];

    // Iteracyjne przesuwanie elementow miedzy i a j-1 w prawo.
    while(i < j-1)
    {
        swap(i+1, i);
        i++;
    }

    route[j-1] = insertedElement;
}
*/

// Metoda pomocnicza - zamiana elementow trasy o indeksach i i j
void Route::swap(unsigned i, unsigned j)
{
    int temp = route[i];
    route[i] = route[j];
    route[j] = temp;
}
