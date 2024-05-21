#include <iostream>
#include <graph.h>

int main(){
    std::string filepath = "./output/";
    std::string ext = ".txt";
    for (int i = 0; i < 200; i++)
    {
        graph g(100,200,6,5);
        g.randomPoint(200);
        g.setDronePosition(10,10);
        g.createMap();  
        // g.printRoute();
        std::string filename = std::to_string(i);
        std::string fullpath = filepath+filename+ext;
        // g.outputRoute(fullpath.c_str());
        g.printRouteScore();
    }
    return 0;
}