#pragma once
#include <pos.h>

struct pos{
    int x,y;
    double dist(pos& _pos);
    double dist(int x,int y);
    void operator = (const pos& _pos);
    void operator += (const pos& _pos);
} ;