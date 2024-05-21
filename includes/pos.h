#pragma once
#include <pos.h>
#include <vector>

struct pos{
    int x,y;
    double dist(pos& _pos);
    double dist(int x,int y);
    void operator = (const pos& _pos);
    void operator += (const pos& _pos);
} ;

struct edge
{
    int u,v;
    double w;
};
bool operator < (const edge& x,const edge& y);

struct point{
    double x,y,w;
    point& operator = (const point& _p);
};


struct point_relations{
    int x=0,y=0;
    double w=0;
    std::vector<int> points;
};