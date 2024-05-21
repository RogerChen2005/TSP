#include <iostream>
#include <graph.h>

int main(){
    std::string filepath = "./output/";
    std::string ext = ".txt";
    for (int i = 0; i < 10; i++)
    {
        graph g(100,200,6,5);
        g.randomPoint(80);
        g.createMap();  
        g.printRoute();
        std::string filename = std::to_string(i);
        std::string fullpath = filepath+filename+ext;
        g.outputRoute(fullpath.c_str());
    }
    return 0;
}