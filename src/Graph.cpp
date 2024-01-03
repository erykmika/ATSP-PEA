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
    try
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
                if(i==j) matrix[i][j] = -1;
                else matrix[i][j] = std::stoi(buf);
            }
        }

        file.close();
    }
    catch(...)
    {
        throw std::string("Wystapil blad podczas otwierania pliku!\n");
    }
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

double Graph::solveGA(unsigned timeLimit, unsigned initialPopulation, double mutationFactor,
                      double crossoverFactor, bool mutationChoice) const
{
    // Dynamiczna tablica - wektor - przechowujaca populacje
    std::vector<Route> population;
    unsigned routeElements = size - 1;
    unsigned eliteSize = initialPopulation;

    Route greedy = generateInitialSolution();
    calculateRouteCost(greedy);
    population.emplace_back(greedy);

    while( population.size() <(unsigned)(0.05d*initialPopulation) )
    {
        // 5% osobnikow w poczatkowej populacji wywodzi sie z rozwiazania zachlannego
        Route r = greedy;
        r.procedureSwap( rand()% routeElements, rand() % routeElements );
        calculateRouteCost(r);
        population.emplace_back(r);
    }

    while( population.size() < initialPopulation )
    {
        // reszta osobnikow jest losowych
        Route r(routeElements);
        r.randomize();
        calculateRouteCost(r);
        population.emplace_back(r);
    }

    std::sort(population.begin(), population.end());

    std::cout<<population[0].getCost()<<"\n";

    unsigned timeCheck = 0;

    std::chrono::duration<double, std::milli> duration;
    std::chrono::duration<double, std::milli> duration1;
    unsigned bestSolution = population[0].getCost();
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();

    // Glowna petla algorytmu
    while(true)
    {
        // Rozmiar populacji w obecnej iteracji
        unsigned populationSize = population.size();

        for(unsigned i=0; i<populationSize; i++)
        {

            // Mutacja
            if( (double)rand() / (double)RAND_MAX < mutationFactor )
            {

                int idx1 = rand() % routeElements;
                int idx2 = rand() % routeElements;
                if(idx1 == idx2) idx2 = ( idx1 + 1 ) % routeElements;
                population[i].mutateInverse(idx1, idx2);

                //population[i].mutateScramble( rand() % (int)routeElements * 0.2 );

                calculateRouteCost(population[i]);
            }

            // Krzyzowanie
            if( (double)rand() / (double)RAND_MAX < crossoverFactor )
            {
                unsigned secIndex = rand()% populationSize;
                if(secIndex == i) secIndex = ( secIndex + 1 ) % populationSize;
                Route offspring = population[i].crossoverPMX( population[secIndex] );
                calculateRouteCost(offspring);
                population.emplace_back(offspring);
            }
        }

        // Sortowanie populacji rozwiazan
        std::sort(population.begin(), population.end());

        if( population[0].getCost() < bestSolution )
        {
            auto end1 = std::chrono::steady_clock::now();
            duration1 = end1 - start;
            if(duration1.count() > timeLimit) break;
            end = end1;
            bestSolution = population[0].getCost();
            std::cout<<bestSolution<<"\n";
        }

        // Zapewnienie, ze populacja ma okreslony rozmiar - eliteSize najbardziej obiecujacych rozwiazan
        if( population.size() > initialPopulation )
        {
            population.erase( population.begin() + eliteSize, population.end() );
        }

        // Sprawdzenie warunku stopu
        if( timeCheck > 10 )
        {
            auto end1 = std::chrono::steady_clock::now();
            duration1 = end1 - start;
            double elapsed_time1 = duration1.count();
            if(elapsed_time1 > timeLimit) break;
            timeCheck = 0;
        }
        timeCheck++;
    }

    // Obliczenie czasu, wyniki
    duration = end - start;
    std::cout<<duration.count()<<" ms; ";
    std::cout<<bestSolution<<"\n";
    return bestSolution;
}

// Obliczanie i ustawianie kosztu sciezki
void Graph::calculateRouteCost(Route& r) const
{
    unsigned cost = matrix[0][r[0]];
    for(int i=1; i<size-1; i++)
    {
        cost += matrix[r[i-1]][r[i]];
    }
    cost += matrix[r[size-2]][0];
    r.setCost(cost);
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

