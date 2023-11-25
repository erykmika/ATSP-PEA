#include <iostream>
#include <random>
#include <time.h>

#include "Graph.h"

#define CASES 100 // Ile losowych instancji dla kazdego N w przypadku badania

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

    /*
        Menu programu zgodne z wytycznymi
    */

    while(!isFinished)
    {
        char choice;

        std::cout<<"1. Wczytaj dane z pliku\n";
        std::cout<<"2. Wprowadz kryterium stopu (sekundy)\n";
        std::cout<<"3. Ustaw wspolczynnik zmiany temperatury\n";
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
        default:
            std::cout<<"Jeszcze niezaimplementowane.\n";
            break;
        }
    }
    return 0;
}
