#pragma once 
#include <vector>
#include <queue>
#include <pos.h>
#include <bmp.h>

class graph
{
private:
    double scan_radius,tw;
    int width,height,flag,drone_count;
    std::priority_queue<edge> egdes;
    std::vector<point> points;
    std::vector<double> point_w;
    std::vector<point> targets;
    std::vector<int> points_to_targets;
    std::vector<std::vector<int>> connect;
    std::vector<int> fa;
    std::vector<int> header;
    std::vector<std::vector<int>> path;
    point _drone;
    void add_edge(int u,int v,double w);
    int findfa(int x);
    bool check(int a,int b);
    void createForest();
    void createBestMatch();
    void createEularRoute();
    void createHamiltonian();
    void calculateRoute();
    void calculateTarget();
    double calculateTotalW(std::vector<int>& _points);
    double calculateEdgeCost(point& p1,point& p2);
public:
    double node_weight,increase_rate;
    graph(int x,int y,int drone_count,double scan_radius);
    void randomPoint(int count);
    void addPoint(const point &p);
    void addPoint(double x,double y,double w);
    void setDronePosition(double x,double y);
    void createMap();
    void printRoute();
    void outputRoute(const char* filename);
    void output(const char* filename);
    void printRouteScore();
};


