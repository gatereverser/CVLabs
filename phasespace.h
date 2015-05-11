#pragma once

struct PhaseSpace {
    int scale;
    int angle;
    int x;
    int y;

    PhaseSpace(int sc, int ang, int i, int j) : scale(sc), angle(ang), x(i), y(j) {}

    bool operator< (const PhaseSpace &p) const {
        if (scale != p.scale)
            return scale < p.scale;
        if (angle != p.angle)
            return angle < p.angle;
        if (x != p.x)
            return x < p.x;
        return y < p.y;
    }

};
