#include "Graph.h"

/*
 Konstruktor - generujemy losowy graf stopnia n do badania ATSP (asymetrycznego problemu komiwoja¿era)
 Oznacza to, ¿e odleg³oœci dla danych wierzcho³ków a, b nie musz¹ byæ takie same w obie strony;
 |ab| niekoniecznie równa siê |ba|.
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

    // Przechodzimy przez 3 pierwsze linie pliku tekstowego (NAME, TYPE, COMMENT)
    std::getline(file, buf);
    std::getline(file, buf);
    std::getline(file, buf);

    std::getline(file, buf, ' ');

    std::getline(file, buf);

    //Pobieramy stopien macierzy z pliku

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
            file >> buf;
            matrix[i][j] = std::stoi(buf);
        }
    }

    file.close();

}

/*
    Metoda do wyswietlania grafu. Iterujemy po wierszach i kolumnach.
    Wypisujemy wartosci krawedzi.
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


Graph::~Graph()
{
    for(int i=0; i<siz; i++){
        delete [] matrix[i];
    }
    delete [] matrix;
}
