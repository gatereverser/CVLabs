#pragma once
#include <math.h>
#include <memory>
#include<map>
#include <QDebug>

#define PIII 3.14159265358979323846
using namespace std;


    class PhaseSpace
    {
    private:
        int dataSize;
        map<long, double> data;

        double maxScale;
        int cntScale;
        int cntAngle;
        double minX, maxX, minY, maxY;
        int cntX, cntY;

        double ds, da, dx, dy;


    private:
        void addInside(int s, int a, int x, int y, double value);

    public:

        void add(double s, double a, double x, double y, double value);
        void normalize();

        void getMax(double &s, double &a, double &x, double &y);
    };

