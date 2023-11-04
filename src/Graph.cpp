#include "Graph.h"

// Konstruktor domyslny - losowy problem dla N=5.
Graph::Graph(): Graph(5) {}

/*
 Konstruktor - generujemy losowy graf stopnia n do badania ATSP (asymetrycznego problemu komiwojazera)
 Oznacza to, ze odleglosci dla danych wierzcholkow a, b nie musza byc takie same w obie strony;
 |ab| niekoniecznie rowna sie |ba|.
*/
Graph::Graph(short int N)
{
    size = N;
    matrix.resize(size, std::vector<short int>(size));

    for(short int i = 0; i < size; i++)
    {
        for(short int j = 0; j < size; j++)
        {
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
    Konstruktor tworzacy reprezentacje grafu na podstawie pliku tekstowego o formacie wg wytycznych do projektu.
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
    matrix.resize(size, std::vector<short int>(size));

    for(short int i=0; i<size; i++)
    {
        for(short int j=0; j<size; j++)
        {
            // Wartosc ciagu znakow do zmiennej pomocniczej buf, zamieniamy na dlugosc krawedzi short int
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
    matrix = sec.matrix;
    size = sec.size;

    return *this;
}

/*
    Metoda do wyswietlania grafu. Iterujemy po wierszach i kolumnach.
    Wypisujemy wartosci dlugosci krawedzi.
*/
void Graph::printGraph(std::vector<std::vector<short int>> matrix)
{
    for(short int i=0; i<(short int)matrix.size(); i++)
    {
        for(short int j=0; j<(short int)matrix.size(); j++)
        {
            std::cout<<matrix[i][j]<<"\t";
        }

        std::cout<<"\n";
    }
}

void Graph::printGraph()
{
    for(short int i=0; i<size; i++)
    {
        for(short int j=0; j<size; j++)
        {
            std::cout<<matrix[i][j]<<"\t";
        }

        std::cout<<"\n";
    }
}

/*
    Glowna metoda programu sluzaca do pomiaru czasu dzialania algorymtu branch-and-bound oraz wyswietlenia najlepszego kosztu
    oraz najlepszej sciezki.
*/
double Graph::timeBranchAndBoundATSP()
{
    // Czy przekroczono limit czasu
    bool timeout = false;
    // Najlepsza znaleziona sciezka
    std::vector<short int>path = {};
    // Rozpoczynamy pomiar czasu
    auto start = std::chrono::system_clock::now();
    // Korzen drzewa rozwiazan metoda branch-and-bound
    BnBNode* root = new BnBNode(0, matrix,  -1, 0, 0, path);
    // Inicjalizujemy stos na wierzcholki, ktore badamy
    std::stack<BnBNode*> st;
    // Umieszczamy korzen na stosie
    st.push(root);
    // Obliczamy gorna granice rozwiazania metoda zachlanna - koszt koncowy nie moze byc wiekszy
    short int upBound = calcUpBnd();

    // Dopoki stos nie jest pusty
    while(!st.empty())
    {
        // Zdejmujemy i usuwamy szczytowy element stosu
        BnBNode* current = st.top();
        st.pop();

        /*
            Jezeli wierzcholek to lisc i jego koszt jest mniejszy od gornej granicy,
            zapisujemy nowa najlepsza sciezke i gorna granice. Dealokujemy pamiec. Kontynuujemy algorytm od kolejnego
            wierzcholka na stosie.
        */
        if(current->isLeaf() && current->getCost()<=upBound)
        {
            path = current->getPath();
            upBound = current->getCost();
            delete current;
            continue;
        }

        // Indeks obecnego wierzcholka
        short int currentNode = current->getNode();

        /*
            Iterujemy po wszystkich mozliwych potomkach danego wezla. Jezeli jego koszt jest mniejszy niz wartosc
            gornego ograniczenia (jest "obiecujacy"), umieszczamy go na stosie. W przeciwnym wypadku - usuwamy.
        */
        for(short int i=0; i<size; i++)
        {
            short int edgeLen = current->getMatrix()[currentNode][i];
            if(edgeLen>=0)
            {
                BnBNode* child = new BnBNode(i, current->getMatrix(), currentNode, current->getCost() + edgeLen,
                                             current->getNumOfVisited(), current->getPath());
                if(child->getCost()<=upBound)
                    st.push(child);
                else delete child;
            }
        }
        // Zwalniamy pamiec po obecnym wierzcholku
        delete current;
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        // Czy przekroczono limit czasu okreslony w pliku naglowkowym
        if(duration.count()>TIME_LIMIT)
        {
            timeout = true;
            break;
        }
    }
    // Koniec pomiaru czasu
    auto end = std::chrono::system_clock::now();

    // Jezeli przekroczono limit czasu - czyscimy pamiec i zwracamy -1
    if(timeout)
    {
        while(!st.empty())
        {
            BnBNode* del = st.top();
            st.pop();
            delete del;
        }
        return -1;
    }
    // Czas, ktory uplynal
    std::chrono::duration<double, std::milli> duration = end - start;
    double elapsed_time = duration.count();

    // Wyswietlenie rezultatow - koszt, sciezka, czas
    std::cout<<"Koszt: "<<std::setw(6)<<upBound<<";\t";
    for(unsigned i=0; i<path.size(); i++)
    {
        std::cout<<path[i];
        if(i<(unsigned)size-1) std::cout<<" -> ";
    }
    std::cout<<"; Czas: "<<std::setw(6)<<elapsed_time<<" ms\n";

    // Zwracamy czas
    return elapsed_time;
}

// Metoda sluzaca do zachlannego obliczenia gornej granicy dla kosztu sciezki
short int Graph::calcUpBnd() const
{
    short int sum = 0;
    // Odwiedzone wierzcholki - dodajemy korzen (0)
    std::vector<short int> visited = {};
    visited.push_back(0);

    // Bedziemy mieli N-1 krawedzi (poza ostatnia)
    for(short int i=0; i<size-1; i++)
    {
        /*
            Dla ostatniego wierzcholka w tablicy znajdujemy inny nieodwiedzony wierzcholek z minimalna wartoscia
            dlugosci krawedzi miedzy tymi wierzcholkami.
        */
        short int dst = 0;
        short int dstMin = SHRT_MAX;
        short int curIndex = visited.back();
        for(short int j=0; j<size; j++)
        {

            if(std::find(visited.begin(), visited.end(), j) == visited.end() && matrix[curIndex][j]<dstMin)
            {
                dst = j;
                dstMin = matrix[curIndex][j];
            }

        }
        // Dodajemy "minimalny" wierzcholek do odwiedzonych, powiêkszamy koszt
        visited.push_back(dst);
        sum+=dstMin;
    }
    // Na koncu wracamy do korzenia
    sum += matrix[visited.back()][0];

    return sum;
}
