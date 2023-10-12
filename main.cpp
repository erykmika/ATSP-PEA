#include <iostream>
#include <random>
#include <time.h>

#include "Graph.h"

/*

Powyzej - importujemy odpowiednie biblioteki potrzebne do interakcji z uzytkownikiem - wczytywanie/odczytywanie,
generowanie liczb pseudolosowych. Importujemy wlasna klase Graph zawierajaca strukture danych grafu oraz badane algorymty.

*/

int main()
{

    //Inicjalizujemy generator liczb pseudolosowych za pomoca wartosci czasu pobranego z systemu

    srand(time(NULL));

    Graph g(5);

    bool isFinished = false;

    while(!isFinished)
    {
        char choice;

        std::cout<<"1. Wczytaj dane z pliku\n";
        std::cout<<"2. Generuj dane losowe\n";
        std::cout<<"3. Wyswietl wczytane/wylosowane dane\n";
        std::cout<<"4. Uruchom algorytm, wyswietl wyniki\n";
        std::cout<<"5. Badania. Uruchom algorytm dla 100 losowych instancji problemu o rozmiarze N\n";
        std::cout<<"6. WYJSCIE\n";
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
            int N;
            std::cout<<"Podaj rozmiar problemu N: ";
            std::cin>>N;
            g = Graph(N);
            break;
        case '3':
            g.printGraph();
            break;
        case '4':
            g.bruteForceTSP();
            break;
        case '6':
            isFinished = true;
            break;
        default:
            std::cout<<"Jeszcze niezaimplementowane.\n";
            break;
        }
    }

    return 0;
}
