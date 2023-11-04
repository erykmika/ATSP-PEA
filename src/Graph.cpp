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

// Drzewo wierzcholkow, kazdy z tablica wskaznikow do wierzcholkow potomnych 1->(n-1)->(n-2)->...->1 na kazdy poziom
double Graph::timeBranchAndBoundATSP()
{
    std::vector<short int>path = {};

    auto start = std::chrono::system_clock::now();
    BnBNode* root = new BnBNode(0, matrix,  -1, 0, 0, path);

    std::priority_queue<BnBNode*, std::vector<BnBNode*>, CmpCost> q;

    q.push(root);
    // std::cout<<calcUpBnd()<<"\n";

    while(!q.empty())
    {
        BnBNode* current = q.top();
        q.pop();


        if(current->isLeaf())
        {
            path = current->getPath();
            std::cout<<"Koszt: "<<std::setw(6)<<current->getCost()<<";\t";
            delete current;
            break;
        }

        short int currentNode = current->getNode();

        for(short int i=0; i<size; i++)
        {
            short int edgeLen = current->getMatrix()[currentNode][i];
            if(edgeLen>=0)
            {
                BnBNode* child = new BnBNode(i, current->getMatrix(), currentNode, current->getCost() + edgeLen,
                                             current->getNumOfVisited(), current->getPath());
                q.push(child);
            }
        }
        delete current;

    }
    auto end = std::chrono::system_clock::now();
    // Konczymy mierzenie czasu. Nie uwzgledniamy wypisywania.

    // Czas, ktory uplynal
    std::chrono::duration<double, std::milli> duration = end - start;
    double elapsed_time = duration.count();

    for(unsigned i=0; i<path.size(); i++)
    {
        std::cout<<path[i];
        if(i<(unsigned)size-1) std::cout<<" -> ";
    }
    std::cout<<"; Czas: "<<std::setw(6)<<elapsed_time<<" ms\n";


    while(!q.empty())
    {
        BnBNode* cur = q.top();
        q.pop();
        delete cur;
    }

    return elapsed_time;
}

double Graph::timeBranchAndBoundATSP2()
{
    std::vector<short int>path = {};
    BnBNode* root = new BnBNode(0, matrix,  -1, 0, 0, path);
    auto start = std::chrono::system_clock::now();
    std::stack<BnBNode*> st;
    st.push(root);
    short int upBound = calcUpBnd();

    while(!st.empty())
    {
        BnBNode* current = st.top();
        st.pop();

        if(current->isLeaf() && current->getCost()<=upBound)
        {
            path = current->getPath();
            upBound = current->getCost();
            delete current;
            continue;
        }

        short int currentNode = current->getNode();

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
        delete current;

    }
    auto end = std::chrono::system_clock::now();
    // Czas, ktory uplynal
    std::chrono::duration<double, std::milli> duration = end - start;
    double elapsed_time = duration.count();

    std::cout<<"Koszt: "<<std::setw(6)<<upBound<<";\t";
    for(unsigned i=0; i<path.size(); i++)
    {
        std::cout<<path[i];
        if(i<(unsigned)size-1) std::cout<<" -> ";
    }
    std::cout<<"; Czas: "<<std::setw(6)<<elapsed_time<<" ms\n";


    /*
    while(!st.empty())
    {
        BnBNode* cur = st.top();
        st.pop();
        std::cout<<"XX\n";
        delete cur;
    }
*/
    return elapsed_time;
}

short int Graph::calcUpBnd() const
{
    short int sum = 0;
    std::vector<short int> visited = {};
    visited.push_back(0);

    for(short int i=0; i<size-1; i++)
    {
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
        visited.push_back(dst);
        sum+=dstMin;
    }
    sum += matrix[visited.back()][0];

    return sum;
}
