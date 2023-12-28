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
    Route r(5);
    std::cout<<r.toString()<<"\n";
    r.mutateScramble(3);
    std::cout<<r.toString()<<"\n";
    */
    Route a(std::vector<int>{1, 4, 2, 8, 5, 7, 3, 6, 9});
    Route b(std::vector<int>{7, 5, 3, 1, 9, 8, 6, 4, 2});
    std::cout<<a.crossoverPMX(b).toString()<<"\n";
    std::cout<<b.crossoverPMX(a).toString()<<"\n";
    // Graf, ktory jest wykorzystywany w poszczegolnych opcjach
    Graph g;

    bool isFinished = false;

    // Kryterium stopu - sekundy
    int seconds = 10;

    while(!isFinished)
    {
        char choice;

        std::cout<<"0. Wyjscie\n";
        std::cout<<"1. Wczytaj dane z pliku\n";
        std::cout<<"2. Wprowadz kryterium stopu (sekundy)\n";
        std::cout<<"3. Ustawienie wielkosci populacji poczatkowej\n";
        std::cout<<"4. Ustawienie wspolczynnika mutacji\n";
        std::cout<<"4. Ustawienie wspolczynnika krzyzowania\n";
        std::cout<<"5. Wybor metody krzyzowania\n";
        std::cout<<"6. Wybor metody mutacji\n";
        std::cout<<"7. Uruchom algorytm\n";
        std::cout<<"-----------------------------------------------------\n";
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
                std::cout<<"\n";
                g.printGraph();
                std::cout<<"\n";
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
        default:
        {
            std::cout<<"Jeszcze niezaimplementowane.\n";
            break;
        }
        }
    }
    return 0;
}
