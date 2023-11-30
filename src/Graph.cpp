#include "Graph.h"

// Konstruktor domyslny - losowy problem dla N=5.
Graph::Graph(): Graph(5) {}

/*
 Konstruktor - generujemy losowy graf stopnia n do badania ATSP (asymetrycznego problemu komiwojazera)
 Oznacza to, ze odleglosci dla danych wierzcholkow a, b nie musza byc takie same w obie strony;
 |ab| niekoniecznie rowna sie |ba|.
*/
Graph::Graph(int N)
{
    size = N;
    // Macierz kwadratowa stopnia N - reprezentacja problemu (A)TSP
    matrix.resize(size, std::vector<int>(size));

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            // Poza przekatna losowa wartosc dlugosci krawedzi z przedzialu [1, 1000]
            if(i != j)
            {
                matrix[i][j] = rand() % 1000 + 1;
            }
            else
            {
                matrix[i][j] = -1;
            }
        }
    }
}

/*
    Konstruktor tworzacy reprezentacje grafu na podstawie pliku tekstowego wg formatu ze
    strony comopt.ifi.uni-heidelberg.de
*/
Graph::Graph(std::string fname)
{
    std::fstream file(fname, std::ios::in);

    if(!file.good())
    {
        throw std::string("Blad otwierania pliku!");
    }

    // String, do ktorego wczytujemy linie z pliku
    std::string buf;

    // Przechodzimy przez pierwsze linie pliku tekstowego (NAME, TYPE, COMMENT, ...)
    for(int i=0; i<3; i++)
        std::getline(file, buf);

    std::getline(file, buf, ' ');

    //Pobieramy stopien macierzy z pliku
    std::getline(file, buf);
    size = std::stoi(buf);

    // Przechodzimy przez kolejne 3 linie
    for(int i=0; i<3; i++)
        std::getline(file, buf);

    // Macierz kwadratowa stopnia N - reprezentacja problemu (A)TSP
    matrix.resize(size, std::vector<int>(size));

    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            // Wartosc ciagu znakow do zmiennej pomocniczej buf, zamieniamy na dlugosc krawedzi int
            file >> buf;
            matrix[i][j] = std::stoi(buf);
        }
    }

    file.close();

}

/*
    Operator przypisania, uzywany do "zamiany" obiektow w glownym programie
*/

Graph& Graph::operator=(const Graph& sec)
{
    // Przypisujemy macierz i rozmiar problemu
    matrix = sec.matrix;
    size = sec.size;

    return *this;
}

// Wyswietlanie macierzy sasiedztwa
void Graph::printGraph() const
{
    // Iterujemy po wierszach i kolumnach
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            std::cout<<matrix[i][j]<<"\t";
        }

        std::cout<<"\n";
    }
}

double Graph::timeSimulatedAnnealing(double delta, int numOfIterations)
{
    int routeElements = size-1;
    //Route currentRoute(routeElements);
    //currentRoute.generateRandom();
    Route currentRoute = generateInitialSolution();
    //std::cout<<currentRoute.toString()<<"\n";
    //int cost = calculateRouteCost(currentRoute);
    //std::cout<<cost<<"\n";
    double T = 100.0; // Temperatura
    Route optimalRoute = currentRoute;
    for(int i=0; i<numOfIterations; i++)
    {
        int pos1 = rand()%routeElements;
        int pos2 = rand()%routeElements;

        Route newRoute = currentRoute;
        newRoute.procedure2opt(pos1, pos2);

        if(((rand()%10000)/(float)10000)<std::min(1.0, std::exp(-(calculateRouteCost(newRoute)-calculateRouteCost(currentRoute))/T)))
        {
            currentRoute = newRoute;
            if(calculateRouteCost(currentRoute)<calculateRouteCost(optimalRoute))
            {
                optimalRoute = currentRoute;
            }
        }
        T *= delta;
    }
    std::cout<<"K = "<<calculateRouteCost(optimalRoute)<<"\n";
    std::cout<<"0 "<<optimalRoute.toString()<<"0 \n";
    return 0.0;
}

double Graph::timeTabuSearch(char neighbourFunction, int numOfIterations)
{
    void (Route::*movePtr)(unsigned, unsigned);

    switch(neighbourFunction)
    {
    case 'n':
        movePtr = &Route::procedureInsert;
        break;
    case 'i':
        movePtr = &Route::procedureInverse;
        break;
    default:
        movePtr = &Route::procedure2opt;
        break;
    }

    int routeElements = size-1;
    Route currentRoute = generateInitialSolution();
    Route optimalRoute = currentRoute;

    for(int it=0; it<numOfIterations; it++)
    {
        Route currentMax;
        int currentMaxVal = INT_MIN;

        // Wszystkie podzbiory 2-elementowe (i, j) - sasiedztwo obecnego rozwiazania
        for(int i=0; i<routeElements; i++)
        {
            for(int j=i+1; j<routeElements; j++)
            {
                Route current = currentRoute;
                (current.*movePtr)(i, j);
                if(calculateRouteCost(currentRoute) - calculateRouteCost(current) > currentMaxVal)
                {
                    currentMaxVal = calculateRouteCost(currentRoute) - calculateRouteCost(current);
                    currentMax = current;
                }
            }
        }
        currentRoute = currentMax;

        if(calculateRouteCost(currentRoute) < calculateRouteCost(optimalRoute))
        {
            optimalRoute = currentRoute;
        }
    }
    std::cout<<"K = "<<calculateRouteCost(optimalRoute)<<"\n";
    std::cout<<"0 "<<optimalRoute.toString()<<"0 \n";
}

int Graph::calculateRouteCost(Route& r) const
{
    int cost = matrix[0][r[0]];
    for(int i=1; i<size-1; i++)
    {
        cost += matrix[r[i-1]][r[i]];
    }
    cost += matrix[r[size-2]][0];
    return cost;
}

// Metoda sluzaca do zachlannego wygenerowania poczatkowego rozwiazania
Route Graph::generateInitialSolution() const
{
    Route res(size-1);
    int sum = 0;
    // Odwiedzone wierzcholki - dodajemy korzen (0)
    std::vector<int> visited = {};
    visited.push_back(0);

    // Bedziemy mieli N-1 krawedzi (poza ostatnia - z powrotem do korzenia)
    for(int i=0; i<size-1; i++)
    {
        /*
            Dla ostatniego wierzcholka w tablicy znajdujemy inny nieodwiedzony wierzcholek z minimalna wartoscia
            dlugosci krawedzi miedzy tymi wierzcholkami.
        */
        int dst = 0;
        int dstMin = INT_MAX;
        int curIndex = visited.back();
        for(int j=0; j<size; j++)
        {
            // Zamiast std::find
            bool isInVisited = false;
            for(unsigned k=0; k < visited.size(); k++)
            {
                if(visited[k]==j)
                {
                    isInVisited = true;
                    break;
                }
            }

            if(!isInVisited && matrix[curIndex][j]<dstMin)
            {
                dst = j;
                dstMin = matrix[curIndex][j];
            }

        }
        // Dodajemy "minimalny" wierzcholek do odwiedzonych, powiekszamy koszt
        visited.push_back(dst);
        res[i] = dst;
    }
    // Na koncu wracamy do korzenia
    sum += matrix[visited.back()][0];

    return res;
}
