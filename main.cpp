#include <iostream>
#include <random>
#include <time.h>

#include "Graph.h"
#include "Route.h"

/*
Powyzej - importujemy odpowiednie biblioteki potrzebne do interakcji z uzytkownikiem - wczytywanie/odczytywanie,
generowanie liczb pseudolosowych. Importujemy wlasna klase Graph zawierajaca/importujaca strukture danych
grafu oraz badane algorymty.
*/

int main()
{
    // Inicjalizujemy generator liczb pseudolosowych za pomoca wartosci czasu pobranego z systemu
    srand(time(NULL));
    /*
    Route r(6);
    r.randomize();
    std::cout<<r.toString()<<"\n";
    r.procedureSwap(0, 5);
    std::cout<<r.toString()<<"\n";
    */
    //Graph t("br17.atsp");
    //t.printGraph();
    //t.solveSimulatedAnnealing();
    /*
    Graph a(4);
    Route b = a.generateInitialSolution();
    a.printGraph();
    std::cout<<b.toString()<<"\n";
    std::cout<<a.calculateRouteCost(b)<<"\n";
    */
    // Graf, ktory jest wykorzystywany w poszczegolnych opcjach
    Graph g;

    bool isFinished = false;

    // Kryterium stopu - sekundy
    int seconds = 10;
    // Wspolczynnik zmiany temperatury dla SA
    double delta = 0.99999;

    char n = 's';

    /*
        Menu programu zgodne z wytycznymi
    */

    /*
    SA
    ftv47.atsp 0.99999
    rbg403.atsp 0.95
    */

    /*
    TS
    ftv47.atsp

    */
    while(!isFinished)
    {
        char choice;

        std::cout<<"0. Wyjscie\n";
        std::cout<<"1. Wczytaj dane z pliku\n";
        std::cout<<"2. Wprowadz kryterium stopu (sekundy)\n";
        std::cout<<"3. Ustaw wspolczynnik zmiany temperatury\n";
        std::cout<<"4. Wybierz sasiedztwo dla TS\n";
        std::cout<<"5. Uruchom algorytm SA (symulowane wyzarzanie) dla wczytanych danych i parametrow\n";
        std::cout<<"6. Uruchom algorytm TS (tabu search) dla wczytanych danych i parametrow\n";
        std::cout<<"------------------------------------------------------------------------------------\n";
        std::cout<<"Wybierz numer opcji: \n";

        std::cin>>choice;

        switch(choice)
        {
        case '0':
        {
            isFinished = true;
            break;
        }
        case '1':
        {
            try
            {
                std::string fname;
                std::cout<<"Podaj nazwe pliku z rozszerzeniem: ";
                std::cin>>fname;
                g = Graph(fname);
            }
            catch(std::string& ex)
            {
                std::cout<<ex<<"\n";
            }
            break;
        }
        case '2':
        {
            std::cout<<"Podaj kryterium stopu (sekundy): ";
            std::cin>>seconds;
            std::cout<<"Ustalono kryterium stopu na "<<seconds<<" sek.\n";
            break;
        }
        case '3':
        {
            std::cout<<"Podaj wspolczynnik zmiany temperatury alfa < 1.0: ";
            std::cin>>delta;
            std::cout<<"Ustalono wspolczynnik zmiany temperatury na "<<delta<<".\n";
            break;
        }
        case '4':
        {
            std::cout<<"Wybierz sasiedztwo: s - swap, n - insert, i - inverse: ";
            std::cin>>n;
            break;
        }
        case '5':
        {
            int repeats = 0;
            unsigned bestResult = UINT_MAX;
            std::cout<<"Podaj ilosc powtorzen: ";
            std::cin>>repeats;
            for(int i=0; i<repeats; i++)
                bestResult = std::min(g.solveSimulatedAnnealing(delta, seconds*1000), bestResult);
            std::cout<<"Najlepsze rozwiazanie metoda SA: "<<bestResult<<".\n";
            break;
        }
        case '6':
        {
            int repeats = 0;
            unsigned bestResult = UINT_MAX;
            std::cout<<"Podaj ilosc powtorzen: ";
            std::cin>>repeats;
            for(int i=0; i<repeats; i++)
                bestResult = std::min(g.solveTabuSearch(n, seconds*1000), bestResult);
            std::cout<<"Najlepsze rozwiazanie metoda SA: "<<bestResult<<".\n";
            break;
        }
        default:
        {
            std::cout<<"Jeszcze niezaimplementowane.\n";
            break;
        }
        }
    }
    return 0;
}
