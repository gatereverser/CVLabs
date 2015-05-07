#pragma once
#include<math.h>
class FeaturePoint
{
    double weight;
    int x;
    int y;
    double scale;
    double orientation;
    int level;

public:
    FeaturePoint (int i, int j, double k) : x(i), y(j), weight(k) {}
    FeaturePoint (int i, int j, double k, double sc) : x(i), y(j), weight(k), scale(sc) {}
    FeaturePoint (int i, int j, double k, double sc, int tlevel, double oriental) : x(i), y(j), weight(k), scale(sc), level(tlevel), orientation(oriental){}
    FeaturePoint (int i, int j, double sc, int tlevel) : x(i), y(j), scale(sc), level(tlevel){}
    inline int getX() { return x;}
    inline int getY() { return y;}
    inline double getScale() { return scale;}
    inline int getOrientation() { return orientation;}
    inline int getLevel() { return level;}
    inline void setX(int i){
        x = i;
    }
    inline void setY(int i){
        y = i;
    }

    inline void setOrientation(int i){
        orientation = i;
    }

    inline void setWeight(double i){
        weight = i;
    }
    inline int getWei() { return weight;}

    inline double getDistance( FeaturePoint other){
        return pow(x - other.getX(), 2) + pow(y - other.getY(), 2);
    }


};


