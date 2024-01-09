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

//Operator przypisania, uzywany do "zamiany" obiektow w glownym programie
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

// Glowna metoda klasy rozwiazujaca problem TSP za pomoca algorytmu genetycznego - zwraca czas [ms] i koszt
std::pair<double, unsigned>  Graph::solveGA(unsigned timeLimit, unsigned initialPopulation, double mutationFactor,
                      double crossoverFactor, bool mutationChoice) const
{
    // Dynamiczna tablica - wektor - przechowujaca populacje
    std::vector<Route> population;
    unsigned routeElements = size - 1;
    // Do kolejnej iteracji jako rodzice przepuszczane jest 50% liczby osobnikow w poczatkowej populacji
    unsigned eliteSize = 0.5 * initialPopulation;

    // Rozwiazanie wygenerowane zachlannie
    Route greedy = generateInitialSolution();
    // Obliczenie kosztu trasy
    calculateRouteCost(greedy);
    // Dodanie trasy do populacji
    population.emplace_back(greedy);

    /*
        10% osobnikow w poczatkowej populacji wywodzi sie z rozwiazania zachlannego
        razem z bazowym rozwiazaniem wygenerowanym zachlannie
    */
    while( population.size() <(unsigned)( 0.1*initialPopulation ) )
    {
        // Wygenerowanie osobnikow pochodzacych z rozwiazania zachlannego - losowe przestawianie elementow trasy
        Route r = greedy;
        r.procedureSwap( rand()% routeElements, rand() % routeElements );
        r.procedureSwap( rand()% routeElements, rand() % routeElements );
        calculateRouteCost(r);
        population.emplace_back(r);
    }

    // Reszta osobnikow jest wygenerowana losowo
    while( population.size() < initialPopulation )
    {
        Route r(routeElements);
        r.randomize();
        calculateRouteCost(r);
        population.emplace_back(r);
    }

    // Sortowanie populacji wg kosztow tras - podejscie rankingowe
    std::sort(population.begin(), population.end());

    unsigned timeCheck = 0;

    // Czas znalezienia najlepszego rozwiazania
    std::chrono::duration<double, std::milli> duration;
    // Czas przerwania algorytmu - warunek stopu
    std::chrono::duration<double, std::milli> duration1;
    // Najlepsze znalezione rozwiazanie
    unsigned bestSolution = population[0].getCost();
    // Zmienne przechowujace wartosc czasu na poczatku i koncu pomiarow
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
                // Mutacja typu scramble
                if( mutationChoice )
                {
                    // Wybierana jest losowa liczba elementow trasy do poprzestawiania
                    population[i].mutateScramble( rand() % routeElements );
                }
                // Mutacja typu inverse
                else
                {
                    // Odwracanie trasy pomiedzy indeksami idx1, idx2
                    int idx1 = 0, idx2 = 0;

                    // Unikniecie braku efektu mutacji
                    while(idx1==idx2)
                    {
                        idx1 = rand() % routeElements;
                        idx2 = rand() % routeElements;
                    }
                    population[i].mutateInverse( idx1, idx2 );
                }
                calculateRouteCost(population[i]);
            }

            // Krzyzowanie
            if( (double)rand() / (double)RAND_MAX < crossoverFactor )
            {
                // Wybranie drugiego osobnika - rozwiazania - do krzyzowania
                unsigned secIndex = rand()% populationSize;
                if(secIndex == i) secIndex = ( secIndex + 1 ) % populationSize;
                Route offspring = population[i].crossoverPMX( population[secIndex] );
                calculateRouteCost(offspring);
                population.emplace_back(offspring);
            }
        }

        std::sort(population.begin(), population.end());

        // Jezeli znaleziono nowe najlepsze rozwiazanie
        if( population[0].getCost() < bestSolution )
        {
            auto end1 = std::chrono::steady_clock::now();
            duration1 = end1 - start;
            if(duration1.count() > timeLimit) break;
            end = end1;
            bestSolution = population[0].getCost();
            //std::cout<<bestSolution<<"\n";
        }

        /*
         Selekcja rodzicow do nastepnej iteracji
         Wybierane jest 'eliteSize' najbardziej obiecujacych (najlepszych) rozwiazan
         */
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

    // Obliczenie czasu
    duration = end - start;
    double timeResult = duration.count();
    //std::cout<<timeResult<<";";
    //std::cout<<bestSolution<<"\n";
    std::pair<double, unsigned> result = {timeResult, bestSolution};
    return result;
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
            // Czy wierzcholek byl juz odwiedzony
            bool isInVisited = false;
            for(unsigned k=0; k < visited.size(); k++)
            {
                if(visited[k]==j)
                {
                    isInVisited = true;
                    break;
                }
            }
            // Jezeli wierzcholek nieodwiedzony i minimum w wierszu
            if(!isInVisited && matrix[curIndex][j]<dstMin)
            {
                // Wyznaczony kolejny wierzcholek na trasie
                dst = j;
                dstMin = matrix[curIndex][j];
            }

        }
        // Dodajemy wyznaczony wierzcholek do odwiedzonych, powiekszamy koszt
        visited.push_back(dst);
        // Wierzcholek dodany do trasy
        res[i] = dst;
    }
    // Na koncu wracamy do korzenia
    sum += matrix[visited.back()][0];

    return res;
}

int Graph::getSize() const
{
    return size;
}
