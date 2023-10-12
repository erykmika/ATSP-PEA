#include <iostream>
#include <random>
#include <time.h>

#include "Graph.h"

/*

Powy¿ej - importujemy odpowiednie biblioteki potrzebne do interakcji z uzytkownikiem - wczytywanie/odczytywanie,
generowanie liczb pseudolosowych. Importujemy w³asn¹ klasê Graph zawierajaca strukture danych grafu oraz badane algorymty.

*/

int main()
{
    /*
     Inicjalizujemy generator liczb pseudolosowych za pomoc¹ wartoœci czasu pobranego z systemu
    */
    srand(time(NULL));
    Graph g("br17.atsp");
    g.printGraph();
    return 0;
}
