#pragma once
#include<math.h>
class FeaturePoint
{
    double weight;
    int x;
    int y;
    double scale;
    double orientation;

public:
    FeaturePoint (int i, int j, double k) : x(i), y(j), weight(k) {}
    FeaturePoint (int i, int j, double k, double sc) : x(i), y(j), weight(k), scale(sc) {}
    FeaturePoint (int i, int j, double k, double sc, double oriental) : x(i), y(j), weight(k), scale(sc), orientation(oriental){}
    inline int getX() { return x;}
    inline int getY() { return y;}
    inline int getScale() { return scale;}
    inline int getOrientation() { return orientation;}
    inline void setX(int i){
        x = i;
    }
    inline void setY(int i){
        y = i;
    }
    inline void setWeight(double i){
        weight = i;
    }
    inline int getWei() { return weight;}
    inline double getDistance( FeaturePoint other){
        return pow(x - other.getX(), 2) + pow(y - other.getY(), 2);
    }
};


