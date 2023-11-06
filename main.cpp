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
        {
            char choice = '1';
            std::cout<<"Bez limitu czasu (1) czy z limitem czasu (2): ";
            std::cin>>choice;
            if(choice=='1')
            {
                g.timeBranchAndBoundATSP();
            }
            else
            {
                std::cout<<"Wybrano opcje z limitem czasu.\n";
                if(g.timeBranchAndBoundATSPlimited() < 0) std::cout<<"Przekroczono limit czasu!\n";
            }
            std::cout<<"\n";
            break;
        }
        case '5':
        {
            int n;
            std::cout<<"Podaj N: ";
            std::cin>>n;
            char choice = '1';
            std::cout<<"Bez limitu czasu (1) czy z limitem czasu (2): ";
            std::cin>>choice;
            if(choice!='1') std::cout<<"Wybrano opcje z limitem czasu.\n";
            double ns = 0;
            int unfinished = 0;
            for(int i=0; i<CASES; i++)
            {
                double currentResult;
                g = Graph(n);
                if(choice=='1')
                    currentResult = g.timeBranchAndBoundATSP();
                else
                    currentResult = g.timeBranchAndBoundATSPlimited();
                // Jezeli przekroczono limit czasu, z metody zwracana jest liczba ujemna.
                if(currentResult < 0) unfinished++;
                else ns += currentResult;
            }
            if(choice=='1')
            {
                std::cout<<"\n\n-------Sredni czas: "<<(int)ns/(CASES)<<" ms.-------\n\n";
            }
            else
            {
                std::cout<<"\n\n-------Sredni czas: "<<(int)ns/(CASES-unfinished)<<" ms.-------\n";
                std::cout<<"-------Przerwano: "<<((float)unfinished/CASES)*100<<"% problemow ("<<unfinished<<").-------\n\n";
            }
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
    return 0;
}
