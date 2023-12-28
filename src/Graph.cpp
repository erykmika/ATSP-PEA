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
    return 0.0d;
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

