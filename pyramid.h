#pragma once
#include<QImage>
#include "cv.h"

using namespace std;

class Pyramid
{
    int octaveCount;
    int levelsCount;
    vector<CVImage> images;
    vector<double> currentSigma;
    vector<double> realSigma;
    vector<int> currentOctave;

public:
    Pyramid(int octaves, int levels);


    void save(const QString &fileName);

    static Pyramid Build(const CVImage &image, int octaveNum, int levelNum, double sigma0Start = 1, double sigmaImage = 0.5);

    double findPixel(int x, int y, float sigma);
};
