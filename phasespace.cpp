#include "phasespace.h"


   PhaseSpace::PhaseSpace(double maxScale, double ds, double da, double minX, double maxX, double dx, double minY, double maxY, double dy) :
        maxScale(maxScale), ds(ds),
        da(da),
        minX(minX), maxX(maxX), dx(dx),
        minY(minY), maxY(maxY), dy(dy)
    {
        cntScale = int(maxScale / ds + 1);
        cntAngle = int((2 * PIII) / da + 1);
        cntX = int((maxX - minX) / dx + 1);
        cntY = int((maxY - minY) / dy + 1);

    }


    void PhaseSpace::addInside(int s, int a, int x, int y, double value)
    {
        if (s >= cntScale || a >= cntAngle || x < 0 || x >= cntX || y < 0 || y >= cntY)
            return;

        long index = 1l * s * cntAngle * cntX * cntY + a * cntX * cntY + x * cntY + y;
        data[index] = value + (data.count(index) ? data[index] : 0);
        qDebug() << data[index];

    }

    void PhaseSpace::add(double s, double a, double x, double y, double value)
    {
        s /= ds;
        a /= da;
        x = (x - minX) / dx;
        y = (y - minY) / dy;

        sum += value;
        value /= 32;

        int is = floor(s) + 0.5;
        double vs[] = {is + 1 - s, 1 - vs[0]};
        int ia = floor(a) + 0.5;
        double va[] = {ia + 1 - a, 1 - va[0]};
        int ix = floor(x) + 0.5;
        double vx[] = {ix + 1 - x, 1 - vx[0]};
        int iy = floor(y) + 0.5;
        double vy[] = {iy + 1 - y, 1 - vy[0]};
        for (int ds = 0; ds <= 1; ++ds)
            for (int da = 0; da <= 1; ++da)
                for (int dx = 0; dx <= 1; ++dx)
                    for (int dy = 0; dy <= 1; ++dy) {
                        addInside(s + ds, a + da, x + dx, y + dy, value * (vs[ds] + va[da] + vx[dx] + vy[dy]));
                    }

    }



    void PhaseSpace::getMax(double &s, double &a, double &x, double &y)
    {
        int maxIndex = max_element(begin(data), end(data), [] (auto &p1, auto &p2) {return p1.second < p2.second;})->first;


        qDebug() << "max data =" <<data[maxIndex];
         qDebug() << "max data =" << data[maxIndex];

        int iy = maxIndex % cntY;
        maxIndex /= cntY;
        int ix = maxIndex % cntX;
        maxIndex /= cntX;
        int ia = maxIndex % cntAngle;
        int is = maxIndex / cntAngle;

        s = is * ds;
        a = ia * da;
        x = ix * dx + minX;
        y = iy * dy + minY;
    }



