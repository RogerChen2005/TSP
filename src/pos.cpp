#include <pos.h>
#include <cmath>

point& point::operator=(const point &_p){
    this->x = _p.x;
    this->y = _p.y;
    this->w = _p.w;
    return *this;
}

bool operator < (const edge& x,const edge& y){
    return x.w < y.w;
}