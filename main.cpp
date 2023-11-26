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
    //Route r(5);
    //r.generateRandom();
    //std::cout<<r.toString()<<"\n";

    //Graph t("br17.atsp");
    //t.printGraph();
    //t.timeSimulatedAnnealing();

    // Graf, ktory jest wykorzystywany w poszczegolnych opcjach
    Graph g;

    bool isFinished = false;

    // Kryterium stopu - sekundy
    int seconds = 120;
    // Wspolczynnik zmiany temperatury dla SA
    double delta = 0.95;

    /*
        Menu programu zgodne z wytycznymi
    */

    while(!isFinished)
    {
        char choice;

        std::cout<<"1. Wczytaj dane z pliku\n";
        std::cout<<"2. Wprowadz kryterium stopu (sekundy)\n";
        std::cout<<"3. Ustaw wspolczynnik zmiany temperatury\n";
        std::cout<<"4. Uruchom algorytm SA dla wczytanych danych i parametrow\n";
        std::cout<<"------------------------------------------------------------------------------------\n";
        std::cout<<"Wybierz numer opcji: \n";

        std::cin>>choice;

        switch(choice)
        {
        case '1':
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
        case '2':
            std::cout<<"Podaj kryterium stopu (sekundy): ";
            std::cin>>seconds;
            std::cout<<"Ustalono kryterium stopu na "<<seconds<<" sek.\n";
            break;
        case '3':
            std::cout<<"Podaj wspolczynnik zmiany temperatury alfa < 1.0: ";
            std::cin>>delta;
            std::cout<<"Ustalono wspolczynnik zmiany temperatury na "<<delta<<".\n";
            break;
        case '4':
            g.timeSimulatedAnnealing(delta);
            break;
        default:
            std::cout<<"Jeszcze niezaimplementowane.\n";
            break;
        }
    }
    return 0;
}
