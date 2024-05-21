#include <iostream>
#include <graph.h>

int main(){
    graph g(100,200,6,5);
    g.randomPoint(80);
    g.createMap();
    g.printRoute();
    return 0;
}