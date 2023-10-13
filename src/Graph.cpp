#include "Graph.h"

// Konstruktor domyslny - losowy problem dla N=5.
Graph::Graph(): Graph(5) {}

/*
 Konstruktor - generujemy losowy graf stopnia n do badania ATSP (asymetrycznego problemu komiwoja¿era)
 Oznacza to, ze odleglosci dla danych wierzcholkow a, b nie musza byc takie same w obie strony;
 |ab| niekoniecznie rowna sie |ba|.
*/
Graph::Graph(int N)
{
    size = N;
    matrix = new int*[size];

    for(int i=0; i<size; i++)
    {
        matrix[i] = new int [size];
        for(int j=0; j<size; j++)
        {
            // Jezeli komorka macierzy nie nalezy do przekatnej, ustawiamy dlugosc krawedzi jako losowa wartosc
            if(i != j)
            {
                matrix[i][j] = rand()%100+1;
            }
            // W przeciwnym razie dlugosc krawedzi = -1 (zgodnie z ustaleniami projektowymi)
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

    // Wczytujemy dlugosci krawedzi
    matrix = new int*[size];

    for(int i=0; i<size; i++)
    {
        matrix[i] = new int [size];
        for(int j=0; j<size; j++)
        {
            // Wartosc ciagu znakow do zmiennej pomocniczej buf, zamieniamy na dlugosc krawedzi int
            file >> buf;
            matrix[i][j] = std::stoi(buf);
        }
    }

    file.close();

}

// Destruktor. Zwalniamy zaalokowana dynamicznie pamiec
Graph::~Graph()
{
    for(int i=0; i<size; i++){
        delete [] matrix[i];
    }
    delete [] matrix;
}

/*
    Operator przypisania, uzywany do "zamiany" obiektow w glownym programie
    Deealokacja i alokacja z przypisaniem odpowiednich wartosci z przypisywanego grafu
*/

Graph& Graph::operator=(const Graph& sec)
{
    for(int i=0; i<size; i++)
    {
        delete [] matrix[i];
    }
    delete [] matrix;

    size = sec.size;
    matrix = new int*[size];

    for(int i=0; i<size; i++)
    {
        matrix[i] = new int [size];
        for(int j=0; j<size; j++)
        {
            matrix[i][j] = sec.matrix[i][j];
        }
    }

    return *this;
}

/*
    Metoda do wyswietlania grafu. Iterujemy po wierszach i kolumnach.
    Wypisujemy wartosci dlugosci krawedzi.
*/
void Graph::printGraph()
{
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            std::cout<<matrix[i][j]<<"\t";
        }

        std::cout<<"\n";
    }
}

/*
    Glowna metoda klasy, sluzaca do znalezienia dlugosci najkrotszego cyklu Hamiltona (rozwiazania problemu TSP),
    ktory spelnia warunki zadania.
*/
void Graph::bruteForceTSP()
{
    int source = 0; // Wierzcholek startowy - wszystkie cykle Hamiltona dla danego wierzcholka sa rownowazne z wszystkimi cyklami H. dla innego
    int permutatedElements = size - 1;  // Permutujemy n-1 elementow (poza pierwszym) - mamy (n-1)! mozliwych cyklow Hamiltona

    // Przygotowujemy 2 tablice - na obecna permutacje oraz na permutacje bedaca najlepsza (dotychczas) znaleziona sciezka
    int* permutation = new int[permutatedElements];
    int* min_solution = new int[permutatedElements];

    // Przygotowanie permutacji, najpierw (0-)1-2...
    for(int i=0; i<permutatedElements; i++)
    {
        permutation[i] = i + 1;
    }

    unsigned int min_cost = UINT_MAX;  // Minimalny koszt - ustawiamy na maks, bedziemy szukac minimum.

    // Korzystajac z funkcji generujacej kolejne permutacje, przechodzimy po wszystkich w petli do..while
    do
    {
        // Obecny koszt - zaczynamy od krawedzi laczacej wierzcholek startowy oraz pierwszy wierzcholek z permutacji n-1 pozostalych
        unsigned int current_cost = matrix[source][permutation[0]];

        // Przechodzimy po grafie wg obecnej permutacji dodajac wagi krawedzi do kosztu
        for(int i=0; i<permutatedElements-1; i++)
        {
            // Dla danego i dodajemy dlugosc krawedzi laczacej wierzcholki i oraz i+1.
            current_cost += matrix[permutation[i]][permutation[i+1]];

            // Jezeli gorzej niz dotychczasowe minimum - przerywamy i kontynnujemy od kolejnej permutacji
            if(current_cost >= min_cost)
                break;
        }

        // Konczymy cykl Hamiltona
        current_cost += matrix[permutation[permutatedElements-1]][source];

        // Jezeli mamy nowe najlepsze rozwiazanie - przypisujemy koszt i zapisujemy sciezke
        if(current_cost < min_cost)
        {
            min_cost = current_cost;

            for(int i=0; i<permutatedElements; i++)
            {
                min_solution[i] = permutation[i];
            }
        }
    }
    while(std::next_permutation(permutation, permutation+permutatedElements));

    std::cout<<"Min cost: "<<min_cost<<"\n";
    std::cout<<source<<" -> ";
    for(int i=0; i<permutatedElements; i++)
    {
        std::cout<<min_solution[i];
        if(i<permutatedElements-1) std::cout<<" -> ";
    }

    std::cout<<"\n";

    delete [] permutation;
    delete [] min_solution;
}
