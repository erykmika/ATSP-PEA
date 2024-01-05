#include <iostream>
#include <random>
#include <time.h>
#include <math.h>
#include <climits>

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

    //Route a(std::vector<int> {10, 9, 6,  5, 3, 7, 8,  1,4, 2});
    //Route b(std::vector<int> {10, 5, 3, 7, 4, 1, 8,  2, 6, 9});
    //std::cout<<a.crossoverPMX(b).toString()<<"\n";
    //std::cout<<b.crossoverPMX(a).toString()<<"\n";
    // Graf, ktory jest wykorzystywany w poszczegolnych opcjach
    Graph g;

    bool isFinished = false;

    // Kryterium stopu - sekundy
    int seconds = 120;
    // Wielkosc populacji poczatkowej
    int initialPopulation = 1e4;
    // Wspolczynnik mutacji
    double mutationFactor = 0.01;
    // Wspolczynnik krzyzowania
    double crossoverFactor = 0.8;
    // Wybor metody mutacji
    bool mutationOption = true;

    //g.printGraph();

    while(!isFinished)
    {
        char choice;

        std::cout<<"0. Wyjscie\n";
        std::cout<<"1. Wczytaj dane z pliku\n";
        std::cout<<"2. Wprowadz kryterium stopu (sekundy)\n";
        std::cout<<"3. Ustawienie wielkosci populacji poczatkowej\n";
        std::cout<<"4. Ustawienie wspolczynnika mutacji\n";
        std::cout<<"5. Ustawienie wspolczynnika krzyzowania\n";
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
                //std::cout<<"\n";
                //g.printGraph();
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
        case '3':
        {
            std::cout<<"Podaj wielkosc populacji poczatkowej: ";
            std::cin>>initialPopulation;
            std::cout<<"Ustalono wielkosc populacji poczatkowej "<<initialPopulation<<".\n";
            break;
        }
        case '4':
        {
            std::cout<<"Podaj wspolczynnik mutacji (0-1): ";
            std::cin>>mutationFactor;
            std::cout<<"Ustalono wspolczynnik mutacji na "<<mutationFactor<<"\n";
            break;
        }
        case '5':
        {
            std::cout<<"Podaj wspolczynnik krzyzowania (0-1): ";
            std::cin>>crossoverFactor;
            std::cout<<"Ustalono wspolczynnik krzyzowania na "<<crossoverFactor<<"\n";
            break;
        }
        case '6':
        {
            std::cout<<"Wybierz metode mutacji (0 - inverse, 1 - scramble): ";
            int mutationChoice = 0;
            std::cin>>mutationChoice;
            mutationOption = (bool)mutationChoice;
            std::string chosenOption = ( (mutationOption) ? "scramble" : "inverse" );
            std::cout<<"Wybrano "<<chosenOption<<".\n";
            break;
        }
        case '7':
        {
            int repeats = 1;
            double timeSum = 0;
            std::cout<<"Podaj liczbe powtorzen: ";
            std::cin>>repeats;
            for(int i=0; i<repeats; i++)
            {
                timeSum += g.solveGA(seconds*1000, initialPopulation, mutationFactor,
                                     crossoverFactor, mutationOption);
            }
            //std::cout<<"Sredni czas: "<<timeSum/(double)repeats<<" ms.\n";
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
