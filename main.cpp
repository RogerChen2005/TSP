#include <iostream>
#include <graph.h>
#include <fstream>

int main(){
    std::string filepath = "./output/";
    std::string ext = ".txt";
    std::ofstream file("./output/statistic.txt", std::ios::out);
    for (double c = 0.5; c < 8; c+=0.5){
        file << c << std::endl;
        for (int j = 0; j < 10; j++)
        {
            graph g(100, 200, 6, 5);
            g.node_weight = c;
            g.randomPoint(200);
            g.setDronePosition(10, 10);
            g.createMap();
            g.outputRouteScore(file);
        }
        // std::string filename = std::to_string(i);
        // std::string fullpath = filepath + filename + ext;
    }
    file.close();
    return 0;
}