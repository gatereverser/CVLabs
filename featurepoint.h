#pragma once
#include<math.h>
class FeaturePoint
{
    double weight;
    int x;
    int y;

public:
    FeaturePoint (int i, int j, double k) : x(i), y(j), weight(k) {}
    inline int getX() { return x;}
    inline int getY() { return y;}
    inline void setX(int i){
        x = i;
    }
    inline void setY(int i){
        y = i;
    }
    inline int getWei() { return weight;}
    inline double getDistance( FeaturePoint other){
        return pow(x - other.getX(), 2) + pow(y - other.getY(), 2);
    }
};


