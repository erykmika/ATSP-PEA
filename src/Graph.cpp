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
    try{
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
    } catch(...) {
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

// Wyznaczanie poczatkowej temperatury dla algorytmu SA
double Graph::generateInitialTemp()
{
    double numOfEdges = size * (size - 1);

    int sum = 0;
    // Obliczenie sredniej dlugosci krawedzi w grafie
    for (int i=0; i < size; i++)
    {
        for (int j=0; j < size; j++)
        {
            if (i!=j)
            {
                sum += matrix[i][j];
            }
        }
    }

    double avg = sum / numOfEdges;
    double diffSum = 0.0;
    // Obliczanie odchylenia przecietnego
    for (int i=0; i < size; i++)
    {
        for (int j=0; j < size; j++)
        {
            if (i != j)
            {
                diffSum += abs(avg - matrix[i][j]);
            }
        }
    }
    // Przyjeto pomnozenie przez stala 10e+5
    return 100000 * (diffSum / numOfEdges);
}

unsigned Graph::solveSimulatedAnnealing(double delta, int timeLimit)
{
    // Inicjalizacja zmiennych czasu i trasy

    int routeElements = size - 1;
    // Czas w ktorym znaleziono najlepsze rozwiazanie
    std::chrono::duration<double, std::milli> duration;
    // Czas przerwania algorytmu
    std::chrono::duration<double, std::milli> duration1;
    // Rozpoczecie pomiaru czasu - wygenerowania rozwiazania zachlannego
    auto start = std::chrono::steady_clock::now();

    // Rozwiazanie zachlanne
    Route currentRoute = generateInitialSolution();

    // Zakonczenie pomiaru czasu
    auto end = std::chrono::steady_clock::now();
    duration = end - start;

    // Obliczenie temp. poczatkowej w oparciu o przetwarzane dane
    double T = generateInitialTemp();

    // Inicjalizacja trasy optymalnej
    Route optimalRoute = currentRoute;
    int optimalCost = calculateRouteCost(optimalRoute);

    // Zmienna kontrolujaca czas
    unsigned timeCheck = 0;

    // Glowna petla algorytmu
    while(true)
    {
        // Losowanie dwoch roznych pozycji w trasie
        int pos1 = rand() % routeElements;
        int pos2 = rand() % routeElements;
        if(pos1 == pos2) pos2 = (pos2 + 1) % routeElements;

        // Tworzenie nowej trasy poprzez zastosowanie operatora swap
        Route newRoute = currentRoute;
        newRoute.procedureInsert(pos1, pos2);

        // Obliczenie kosztów obecnej i nowej trasy
        int currentCost = calculateRouteCost(currentRoute);
        int newCost = calculateRouteCost(newRoute);

        // Obliczenie roznicy kosztow miedzy trasami
        int costDiff =  newCost - currentCost;

        // Sprawdzenie czy nowa trasa jest lepsza lub czy zaakceptowac gorsze rozwiazanie
        if(costDiff < 0)
        {
            currentRoute = newRoute;
        }
        else if( ( (double)rand() / (double)RAND_MAX ) <= exp( -(double)costDiff/T ) )
        {
            currentRoute = newRoute;
            currentCost = newCost;
        }

        // Aktualizacja trasy optymalnej, jesli znaleziono lepsze rozwiazanie
        if(currentCost < optimalCost)
        {
            optimalRoute = currentRoute;
            optimalCost = currentCost;
            std::cout<<optimalCost<<"\n";
            // Aktualizacja czasu
            auto end = std::chrono::steady_clock::now();
            duration = end - start;
        }

        // Zmniejszanie temperatury
        T *= delta;
        timeCheck++;

        // Sprawdzenie czasu i przerwanie petli, jesli przekroczony limit czasu
        if(timeCheck > 100000)
        {
            auto end1 = std::chrono::steady_clock::now();
            duration1 = end1 - start;
            double elapsed_time1 = duration1.count();
            if(elapsed_time1 > timeLimit) break;
            timeCheck = 0;
        }
    }

    // Zakonczenie pomiaru calkowitego czasu i wypisanie wyników
    double elapsed_time = duration.count();
    std::cout << elapsed_time << "; ";
    std::cout << optimalCost << ";";
    std::cout << optimalRoute.toString() << ";" << T << "\n";

    // Zwrocenie optymalnego kosztu
    return optimalCost;
}


unsigned Graph::solveTabuSearch(char neighbourFunction, int timeLimit)
{
    // Wskaznik na metode ruchu/sasiedztwa
    void (Route::*movePtr)(unsigned, unsigned);
    // Rozmiar listy tabu
    unsigned tabuListSize = size;
    // Lista elementow trasy
    int routeElements = size-1;
    // Wybor definicji sasiedztwa
    switch(neighbourFunction)
    {
    case 'n':
        movePtr = &Route::procedureInsert;
        break;
    case 'i':
        movePtr = &Route::procedureInverse;
        break;
    default:
        movePtr = &Route::procedureSwap;
        break;
    }

    // Licznik krytyczny (dywersyfikacji)
    int criticalBound = 100000/size;
    // Pomiar czasu - generowania rozwiazania zachlannego
    auto start = std::chrono::steady_clock::now();
    Route currentRoute = generateInitialSolution();
    auto end = std::chrono::steady_clock::now();

    Route optimalRoute = currentRoute;
    int optimalCost = calculateRouteCost(optimalRoute);

    std::vector<std::pair<int, int>>tabuList;

    int criticalCounter = 0;

    // Czas w ktorym znaleziono najlepsze rozwiazanie
    std::chrono::duration<double, std::milli> duration;
    // Czas przerwania algorytmu
    std::chrono::duration<double, std::milli> duration1;

    // Element tabu nr 1 - indeks i element pod tym indeksem
    std::pair<int,int> currentTabuFirst;
    // Element tabu nr 2 - indeks i element pod tym indeksem
    std::pair<int,int> currentTabuSecond;

    // Zmienna kontrolujaca czas
    unsigned timeCheck = 0;

    while(true)
    {
        // Dywersyfikacja
        if(criticalCounter >= criticalBound)
        {
            currentRoute.randomize();
            criticalCounter = 0;
            tabuList.clear();
        }

        int currentCost = calculateRouteCost(currentRoute);
        // Najlepszy sasiad
        Route currentBest = currentRoute;
        int currentMaxVal = INT_MIN;

        // Inicjalizacja tabu
        std::pair<int, int> forbiddenFirst(-1, -1);
        std::pair<int, int> forbiddenSecond(-1, -1);

        bool newTabu = false;

        // Wszystkie podzbiory 2-elementowe (i, j) - sasiedztwo obecnego rozwiazania
        for(int i=0; i<routeElements; i++)
        {

            for(int j=i+1; j<routeElements; j++)
            {
                // Wykonanie ruchu
                Route newRoute = currentRoute;

                (newRoute.*movePtr)(i, j);

                int newCost = calculateRouteCost(newRoute);

                currentTabuFirst = std::pair<int, int>(i, newRoute[i]);
                currentTabuSecond = std::pair<int, int>(j, newRoute[j]);

                bool isTabu = false;

                // Sprawdzenie czy obecny ruch jest tabu
                for(unsigned k=0; k<tabuList.size(); k++)
                {
                    // Sprawdzenie czy ruch jest tabu
                    if( tabuList[k] == currentTabuFirst || tabuList[k] == currentTabuSecond )
                    {
                        isTabu = true;
                        break;
                    }
                }

                // Czy nie jest tabu i jest nowym lokalnym najlepszym rozwiazaniem lub spelnione jest kryterium aspiracji
                if(( !isTabu && ( currentCost - newCost > currentMaxVal ))|| newCost < optimalCost )
                {
                    currentMaxVal = currentCost - newCost;
                    currentBest = newRoute;
                    forbiddenFirst = currentTabuFirst;
                    forbiddenSecond = currentTabuSecond;
                    newTabu = true;
                }
            }
        }

        // Jezeli mamy nowe tabu - dodawane jest ono do listy
        if(newTabu)
        {
            tabuList.push_back(forbiddenFirst);
            tabuList.push_back(forbiddenSecond);
        }

        // Zapewnienie stalej dlugosci listy tabu
        while(tabuList.size() > tabuListSize)
        {
            tabuList.erase(tabuList.begin());
        }

        // Zamiana trasy po wykonanym ruchu
        currentRoute = currentBest;
        currentCost = calculateRouteCost(currentRoute);

        // Czy jest nowe najlepsze rozwiazanie
        if(currentCost < optimalCost)
        {
            end = std::chrono::steady_clock::now();
            duration = end - start;
            if(duration.count()>timeLimit) break;
            optimalRoute = currentRoute;
            optimalCost = currentCost;
            std::cout<<optimalCost<<"\n";
            criticalCounter = 0;
        }

        // Sprawdzenie czasu i przerwanie petli, jesli przekroczony limit czasu
        if(timeCheck > 10)
        {
            auto end1 = std::chrono::steady_clock::now();
            duration1 = end1 - start;
            if(duration1.count() > timeLimit) break;
            timeCheck = 0;
        }

        timeCheck++;
        criticalCounter++;
    }

    // Zakonczenie pomiaru calkowitego czasu i wypisanie wyników
    duration = end - start;
    double elapsed_time = duration.count();
    std::cout << elapsed_time << "; ";
    std::cout << optimalCost << ";";
    std::cout << optimalRoute.toString() << ";\n";
    // Zwrocenie optymalnego kosztu
    return optimalCost;
}

// Obliczanie kosztu sciezki
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
