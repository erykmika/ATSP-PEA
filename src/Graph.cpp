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
    matrix = new int*[size];

    for(int i=0; i<size; i++)
    {
        matrix[i] = new int [size];
        for(int j=0; j<size; j++)
        {
            // Jezeli komorka macierzy nie nalezy do przekatnej, ustawiamy dlugosc krawedzi jako losowa wartosc
            if(i != j)
            {
                matrix[i][j] = rand()%1000+1;
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
    Konstruktor tworzacy reprezentacje grafu na podstawie pliku tekstowego wg wytycznych do projektu.
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

    //Pobieramy stopien macierzy z pliku
    std::getline(file, buf);
    size = std::stoi(buf);

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
    for(int i=0; i<size; i++)
    {
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
void Graph::printGraph(int** matrix, int n)
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            std::cout<<matrix[i][j]<<"\t";
        }

        std::cout<<"\n";
    }
}

// Drzewo wierzcholkow, kazdy z tablica wskaznikow do wierzcholkow potomnych 1->(n-1)->(n-2)->...->1 na kazdy poziom
void Graph::branchAndBoundATSP()
{
    BnBTree t(this->matrix, size);
    int lowerBound = t.getRoot()->reduceMatrix();
    Graph::printGraph(t.getRoot()->getMatrix(), size);
}
