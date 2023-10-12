#include <iostream>
#include <random>
#include <time.h>

#include "Graph.h"

/*

Powy�ej - importujemy odpowiednie biblioteki potrzebne do interakcji z uzytkownikiem - wczytywanie/odczytywanie,
generowanie liczb pseudolosowych. Importujemy w�asn� klas� Graph zawierajaca strukture danych grafu oraz badane algorymty.

*/

int main()
{
    /*
     Inicjalizujemy generator liczb pseudolosowych za pomoc� warto�ci czasu pobranego z systemu
    */
    srand(time(NULL));
    Graph g("br17.atsp");
    g.printGraph();
    return 0;
}
