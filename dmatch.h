#include "featurepoint.h"
#pragma once

struct Dmatch{
    double distance = INFINITY;
    int firstMatch = -1;
    int secondMatch = -1;
    int bin  = -1;
    Dmatch (int i, int j, double k) : firstMatch(i), secondMatch(j), distance(k) {}
};

