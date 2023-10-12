#include "Graph.h"

/*
 Konstruktor - generujemy losowy graf stopnia n do badania ATSP (asymetrycznego problemu komiwoja¿era)
 Oznacza to, ze odleglosci dla danych wierzcholkow a, b nie musza byc takie same w obie strony;
 |ab| niekoniecznie rowna sie |ba|.
*/
Graph::Graph(int N)
{
    siz = N;
    matrix = new int*[siz];

    for(int i=0; i<siz; i++)
    {
        matrix[i] = new int [siz];
        for(int j=0; j<siz; j++)
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
    std::getline(file, buf);
    std::getline(file, buf);
    std::getline(file, buf);
    std::getline(file, buf, ' ');

    //Pobieramy stopien macierzy z pliku

    std::getline(file, buf);
    siz = std::stoi(buf);

    // Przechodzimy przez kolejne 3 linie

    std::getline(file, buf);
    std::getline(file, buf);
    std::getline(file, buf);


    // Wczytujemy dlugosci krawedzi

    matrix = new int*[siz];

    for(int i=0; i<siz; i++)
    {
        matrix[i] = new int [siz];
        for(int j=0; j<siz; j++)
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
    Deealokacja i alokacja z przypisaniem odpowiednich wartosci z przypisywanego grafu
*/

Graph& Graph::operator=(const Graph& sec)
{
    for(int i=0; i<siz; i++)
    {
        delete [] matrix[i];
    }
    delete [] matrix;

    siz = sec.siz;
    matrix = new int*[siz];

    for(int i=0; i<siz; i++)
    {
        matrix[i] = new int [siz];
        for(int j=0; j<siz; j++)
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
    for(int i=0; i<siz; i++)
    {
        for(int j=0; j<siz; j++)
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
    // Przygotowujemy 2 tablice - na obecna permutacje oraz na permutacje bedaca najlepsza (dotychczas) znaleziona sciezka

    int permutation[siz];
    int min_solution[siz];

    // Przygotowanie permutacji, najpierw 0-1-2...

    for(int i=0; i<siz; i++)
    {
        permutation[i] = i;
    }

    unsigned long long min_cost = INT_MAX;

    // Korzystajac z funkcji generujacej kolejne permutacje, przechodzimy po wszystkich w petli do..while

    do
    {
        // Czy mozna przerwac sprawdzanie - obecny koszt na pewno wiekszy od dotychczas minimalnego
        bool isWorse = false;

        // Obecny koszt
        unsigned long long current_cost = 0;

        // Przechodzimy po grafie wg obecnej permutacji dodajac wagi krawedzi do kosztu
        for(int i=0; i<siz; i++)
        {
            // Dla danego i dodajemy dlugosc krawedzi laczacej wierzcholki i oraz i+1. Dla ostatniego operacja modulo gwarantuje powrot do poczatku.
            current_cost += matrix[permutation[i]][permutation[(i+1)%siz]];

            // Jezeli gorzej niz dotychczasowe minimum - przerywamy i kontynnujemy od kolejnej permutacji
            if(current_cost > min_cost)
            {
                isWorse = true;
                break;
            }
        }

        if(isWorse) continue;

        // Jezeli mamy nowe najlepsze rozwiazanie - przypisujemy koszt i zapisujemy sciezke

        if(current_cost < min_cost)
        {
            min_cost = current_cost;

            for(int i=0; i<siz; i++)
            {
                min_solution[i] = permutation[i];
            }
        }
    }
    while(std::next_permutation(permutation, permutation+siz));

    std::cout<<"Min cost: "<<min_cost<<"\n";
    for(int i=0; i<siz; i++)
    {
        std::cout<<min_solution[i];
        if(i<siz-1) std::cout<<" -> ";
    }

    std::cout<<"\n";
}

// Destruktor. Zwalniamy zaalokowana dynamicznie pamiec
Graph::~Graph()
{
    for(int i=0; i<siz; i++){
        delete [] matrix[i];
    }
    delete [] matrix;
}
