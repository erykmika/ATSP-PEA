#include <iostream>
#include "Graph.h"

int main()
{
    /*
    std::cout << "Hello world!\n";
    Graph g(10);
    g.printGraph();
    */
    Graph g("reduction6.atsp");
    g.branchAndBoundATSP();
    //g.printGraph();
    return 0;
}
