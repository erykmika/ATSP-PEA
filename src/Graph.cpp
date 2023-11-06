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

/*
    Glowna metoda programu sluzaca do pomiaru czasu dzialania algorymtu branch-and-bound oraz wyswietlenia najlepszego kosztu
    oraz najlepszej sciezki (BEZ LIMITU CZASU WYKONYWANIA ALGORYTMU).
*/
double Graph::timeBranchAndBoundATSP() const
{
    // Najlepsza znaleziona sciezka
    std::vector<int>path = {};
    // Rozpoczynamy pomiar czasu
    auto start = std::chrono::steady_clock::now();
    // Korzen drzewa rozwiazan metoda branch-and-bound
    BnBNode* root = new BnBNode(0, matrix,  -1, 0, path);
    // Inicjalizujemy stos na wierzcholki, ktore badamy
    BnBStack st;
    // Umieszczamy korzen na stosie
    st.push(root);
    // Obliczamy gorna granice rozwiazania metoda zachlanna - koszt koncowy nie moze byc wiekszy
    int upBound = calcUpBnd();

    // Dopoki stos nie jest pusty
    while(!st.isEmpty())
    {
        // Zdejmujemy i usuwamy szczytowy element stosu
        BnBNode* current = st.peek();
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
        int currentNode = current->getNode();

        /*
            Iterujemy po wszystkich mozliwych potomkach danego wezla. Jezeli jego koszt jest mniejszy niz wartosc
            gornego ograniczenia (jest "obiecujacy"), umieszczamy go na stosie. W przeciwnym wypadku - usuwamy.
        */
        for(int i=0; i<size; i++)
        {
            int edgeLen = current->getMatrix()[currentNode][i];
            if(edgeLen>=0)
            {
                BnBNode* child = new BnBNode(i, current->getMatrix(), currentNode, current->getCost() + edgeLen, current->getPath());
                if(child->getCost()<=upBound)
                    st.push(child);
                else delete child;
            }
        }
        // Zwalniamy pamiec po obecnym wierzcholku
        delete current;
    }
    // Koniec pomiaru czasu
    auto end = std::chrono::steady_clock::now();

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

    // Zwracamy czas w milisekundach
    return elapsed_time;
}

/*
    Ta sama metoda, co wyzej, ale z LIMITEM CZASU ustawionym w pliku naglowkowym Graph.h jako makro
    TIME_LIMIT - pomiar czasu ma wplyw na czas wykonywania algorymtu oraz limit jest niedokladny;
    sprawdzenie limitu co 1000 wykonan petli while - w przeciwnym razie pomiar czasu dzialania algorytmu
    bylby znaczaco zawyzony.
*/
double Graph::timeBranchAndBoundATSPlimited() const
{
    // Czy przekroczono limit czasu
    bool timeout = false;
    std::chrono::duration<double, std::milli> duration;

    // Najlepsza znaleziona sciezka
    std::vector<int>path = {};
    // Rozpoczynamy pomiar czasu
    auto start = std::chrono::steady_clock::now();
    // Korzen drzewa rozwiazan metoda branch-and-bound
    BnBNode* root = new BnBNode(0, matrix,  -1, 0, path);
    // Inicjalizujemy stos na wierzcholki, ktore badamy
    BnBStack st;
    // Umieszczamy korzen na stosie
    st.push(root);
    // Obliczamy gorna granice rozwiazania metoda zachlanna - koszt koncowy nie moze byc wiekszy
    int upBound = calcUpBnd();

    unsigned long long terminationCheck = 1;
    // Dopoki stos nie jest pusty
    while(!st.isEmpty())
    {
        // Sprawdzaj czy limit przekroczony co 1000 wykonan petli
        if(terminationCheck%1000)
        {
            auto end = std::chrono::steady_clock::now();
            duration = end - start;
            // Czy przekroczono limit czasu okreslony w pliku naglowkowym
            if(duration.count()>TIME_LIMIT)
            {
                timeout = true;
                break;
            }
        }
        // Zdejmujemy i usuwamy szczytowy element stosu
        BnBNode* current = st.peek();
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
        int currentNode = current->getNode();

        /*
            Iterujemy po wszystkich mozliwych potomkach danego wezla. Jezeli jego koszt jest mniejszy niz wartosc
            gornego ograniczenia (jest "obiecujacy"), umieszczamy go na stosie. W przeciwnym wypadku - usuwamy.
        */
        for(int i=0; i<size; i++)
        {
            int edgeLen = current->getMatrix()[currentNode][i];
            if(edgeLen>=0)
            {
                BnBNode* child = new BnBNode(i, current->getMatrix(), currentNode, current->getCost() + edgeLen, current->getPath());
                if(child->getCost()<=upBound)
                    st.push(child);
                else delete child;
            }
        }
        // Zwalniamy pamiec po obecnym wierzcholku
        delete current;
        terminationCheck++;
    }
    // Koniec pomiaru czasu
    auto end = std::chrono::steady_clock::now();

    // Jezeli przekroczono limit czasu - zwalniamy pamiec i zwracamy -1
    if(timeout)
    {
        while(!st.isEmpty())
        {
            BnBNode* del = st.peek();
            st.pop();
            delete del;
        }
        return -1;
    }

    // Czas, ktory uplynal
    duration = end - start;

    double elapsed_time = duration.count();

    // Wyswietlenie rezultatow - koszt, sciezka, czas
    std::cout<<"Koszt: "<<std::setw(6)<<upBound<<";\t";
    for(unsigned i=0; i<path.size(); i++)
    {
        std::cout<<path[i];
        if(i<(unsigned)size-1) std::cout<<" -> ";
    }
    std::cout<<"; Czas: "<<std::setw(6)<<elapsed_time<<" ms\n";

    // Zwracamy czas w milikseundach
    return elapsed_time;
}

// Metoda sluzaca do zachlannego obliczenia gornej granicy dla kosztu sciezki
int Graph::calcUpBnd() const
{
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
                if(visited[k]==j) {
                    isInVisited = true;
                    break;
                }
            }

            if(/*std::find(visited.begin(), visited.end(), j) == visited.end()*/ !isInVisited && matrix[curIndex][j]<dstMin)
            {
                dst = j;
                dstMin = matrix[curIndex][j];
            }

        }
        // Dodajemy "minimalny" wierzcholek do odwiedzonych, powiekszamy koszt
        visited.push_back(dst);
        sum += dstMin;
    }
    // Na koncu wracamy do korzenia
    sum += matrix[visited.back()][0];

    return sum;
}
