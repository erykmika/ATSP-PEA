#include <iostream>
#include "Graph.h"

int main()
{
    /*
    std::cout << "Hello world!\n";
    Graph g(10);
    g.printGraph();
    */
    Graph g("reduction5.atsp");
    //g.printGraph();
    std::cout<<"\n\n";
    g.branchAndBoundATSP();
    //g.printGraph();
    return 0;
}
