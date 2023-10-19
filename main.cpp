#include <iostream>
#include <random>
#include <time.h>

#include "Graph.h"
#include "PermutationArray.h"

#define CASES 100 // Ile losowych instancji dla kazdego N w przypadku badania

//#define PERM_TEST

/*
Powyzej - importujemy odpowiednie biblioteki potrzebne do interakcji z uzytkownikiem - wczytywanie/odczytywanie,
generowanie liczb pseudolosowych. Importujemy wlasne klasy Graph oraz PermutationArray zawierajace strukture danych
grafu oraz badane algorymty.
*/

int main()
{

// Test klasy generujacej kolejne leksykograficzne permutacje

#ifdef PERM_TEST
    PermutationArray arr(4);

    for(int i=0; i<24; i++)
    {
        for(int j=0; j<4; j++)
        {
            std::cout<<arr[j]<<" ";
        }
        std::cout<<"\n";
        arr.nextPermutation();
    }
#endif

#ifndef PERM_TEST
    //Inicjalizujemy generator liczb pseudolosowych za pomoca wartosci czasu pobranego z systemu

    srand(time(NULL));

    // Graf, ktory jest wykorzystywany i modyfikowany w opcjach 1, 2, 3, 4
    Graph g;

    bool isFinished = false;

    /*
        Menu programu zgodne z wytycznymi oraz dodatkowo opcja umozliwiajaca zbiorcze
        badanie dla wielu instancji problemu o rozmiarze N
    */

    while(!isFinished)
    {
        char choice;

        std::cout<<"1. Wczytaj dane z pliku\n";
        std::cout<<"2. Generuj dane losowe\n";
        std::cout<<"3. Wyswietl wczytane/wylosowane dane\n";
        std::cout<<"4. Uruchom algorytm, wyswietl wyniki\n";
        std::cout<<"5. Badania. Uruchom algorytm dla "<<CASES<<" losowych instancji problemu o rozmiarze N\n";
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
            g.measureBruteForceATSP();
            std::cout<<"\n";
            break;
        case '5':
        {
            int n;
            std::cout<<"Podaj N: ";
            std::cin>>n;
            double ns = 0;
            for(int i=0; i<CASES; i++){
                g = Graph(n);
                ns += g.measureBruteForceATSP();
                std::cout<<"\n";
            }
            std::cout<<"\n\n-------Sredni czas: "<<std::setprecision(10)<<ns/CASES<<" us.-------\n\n";
            break;
        }
        case '6':
            isFinished = true;
            break;
        default:
            std::cout<<"Jeszcze niezaimplementowane.\n";
            break;
        }
    }
#endif
    return 0;
}
