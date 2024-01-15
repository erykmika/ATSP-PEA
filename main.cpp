#include <iostream>
#include <random>
#include <time.h>
#include <math.h>
#include <climits>
#include <fstream>

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

    // Graf, ktory jest wykorzystywany w poszczegolnych opcjach
    Graph g;

    bool isFinished = false;

    // Kryterium stopu - sekundy
    int seconds = 120;
    // Wielkosc populacji poczatkowej
    int initialPopulation = 1e3;
    // Wspolczynnik mutacji
    double mutationFactor = 0.01;
    // Wspolczynnik krzyzowania
    double crossoverFactor = 0.8;
    // Wybor metody mutacji - domyslnie inverse
    bool mutationOption = false;

    // Parametry do badan zbiorczych
    std::string files[3] = {"ftv47.atsp", "ftv170.atsp", "rbg403.atsp"};
    unsigned timeArr[3] = {120, 240, 360};
    double mutFactors[3] = {0.02, 0.05, 0.1};

    std::string chosenOption = ( (mutationOption) ? "scramble" : "inverse" );

    while(!isFinished)
    {
        char choice;

        std::cout<<"0. Wyjscie\n";
        std::cout<<"1. Wczytaj dane z pliku\n";
        std::cout<<"2. Wypisz wczytane dane\n";
        std::cout<<"3. Wprowadz kryterium stopu (sekundy)\n";
        std::cout<<"4. Ustawienie wielkosci populacji poczatkowej\n";
        std::cout<<"5. Ustawienie wspolczynnika mutacji\n";
        std::cout<<"6. Ustawienie wspolczynnika krzyzowania\n";
        std::cout<<"7. Wybor metody mutacji\n";
        std::cout<<"8. Uruchom algorytm\n";
        std::cout<<"a. Zbiorcze badanie dla pokolen, wsp. mutacji = 0.01, rozne populacje - zapis do plikow\n";
        std::cout<<"b. Zbiorcze badanie dla pokolen, rozne wsp. mutacji, populacja 10e5 - zapis do plikow\n";
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
            }
            catch(std::string& ex)
            {
                std::cout<<ex<<"\n";
            }
            break;
        }
        case '2':
        {
            std::cout<<"\n";
            g.printGraph();
            std::cout<<"\n";
            break;
        }
        case '3':
        {
            std::cout<<"Podaj kryterium stopu (sekundy): ";
            std::cin>>seconds;
            std::cout<<"Ustalono kryterium stopu na "<<seconds<<" sek.\n";
            break;
        }
        case '4':
        {
            std::cout<<"Podaj wielkosc populacji poczatkowej: ";
            std::cin>>initialPopulation;
            std::cout<<"Ustalono wielkosc populacji poczatkowej "<<initialPopulation<<".\n";
            break;
        }
        case '5':
        {
            std::cout<<"Podaj wspolczynnik mutacji (0-1): ";
            std::cin>>mutationFactor;
            std::cout<<"Ustalono wspolczynnik mutacji na "<<mutationFactor<<"\n";
            break;
        }
        case '6':
        {
            std::cout<<"Podaj wspolczynnik krzyzowania (0-1): ";
            std::cin>>crossoverFactor;
            std::cout<<"Ustalono wspolczynnik krzyzowania na "<<crossoverFactor<<"\n";
            break;
        }
        case '7':
        {
            std::cout<<"Wybierz metode mutacji (0 - inverse, 1 - scramble): ";
            int mutationChoice = 0;
            std::cin>>mutationChoice;
            mutationOption = (bool)mutationChoice;
            chosenOption = ( (mutationOption) ? "scramble" : "inverse" );
            std::cout<<"Wybrano "<<chosenOption<<".\n";
            break;
        }
        case '8':
        {
            {
                int repeats = 1;
                double timeSum = 0;
                std::cout<<"Podaj liczbe powtorzen: ";
                std::cin>>repeats;
                for(int i=0; i<repeats; i++)
                {
                    std::pair<double, unsigned> result = g.solveGA(seconds*1000, initialPopulation, mutationFactor,
                                                         crossoverFactor, mutationOption);
                    timeSum += result.first;
                    std::cout<<"t="<<result.first<<" ms, k="<<result.second<<"\n";

                }
                std::cout<<"Sredni czas: "<<timeSum/(double)repeats<<" ms.\n";
            }
            break;
        }

        // Zbiorcze badania
        case 'a':
        {
            std::string files[3] = {"ftv47.atsp", "ftv170.atsp", "rbg403.atsp"};
            for(int f=0; f<3; f++)
            {
                g = Graph(files[f]);
                bool mutationOptionX = false;
                for(int i=0; i<2; i++)
                {
                    // Populacje poczatkowe 10e3 do 10e5
                    for(int j=1000; j<=100000; j*=10)
                    {
                        std::string mutationChoice = ((mutationOptionX) ? "scramble" : "inverse");
                        std::string recordName = std::to_string(g.getSize()) + mutationChoice + std::to_string(j) + ".csv";
                        std::fstream recordFile(recordName, std::ios::out | std::ios::app);
                        std::cout<<g.getSize()<<";"<<mutationChoice<<";"<<j<<"\n";
                        recordFile<<g.getSize()<<";"<<mutationChoice<<";"<<j<<"\n";
                        recordFile.close();
                        g.solveGA(timeArr[f]*1000, j, mutationFactor, crossoverFactor, mutationOptionX, recordName);
                    }
                    mutationOptionX = !mutationOptionX;
                }
            }
            break;
        }
        case 'b':
        {
            for(int f=0; f<3; f++)
            {
                g = Graph(files[f]);
                bool mutationOptionX = false;
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<3; j++)
                    {
                        std::string mutationChoice = ((mutationOptionX) ? "scramble" : "inverse");
                        std::string recordName = std::to_string(g.getSize()) + mutationChoice + std::to_string(mutFactors[j]) + "mut.csv";
                        std::fstream recordFile(recordName, std::ios::out | std::ios::app);
                        std::cout<<g.getSize()<<";"<<mutationChoice<<";"<<mutFactors[j]<<"\n";
                        recordFile<<g.getSize()<<";"<<mutationChoice<<";"<<j<<"\n";
                        recordFile.close();
                        g.solveGA(timeArr[f]*1000, 1e5, mutFactors[j], crossoverFactor, mutationOptionX, recordName);
                    }
                    mutationOptionX = !mutationOptionX;
                }
            }
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
