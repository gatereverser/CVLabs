#pragma once
#include<QImage>
#include "cv.h"

using namespace std;

class Pyramid
{
    int octaveCount;
    int levelsCount;

    vector<double> currentSigma;
    vector<int> currentOctave;
    vector<double> realSigma;
    vector<CVImage> images;

public:
    Pyramid(int octaves, int levels);


    void save(const QString &fileName);

    static Pyramid Build(const CVImage &image, int octaveNum, int levelNum, double sigma0Start = 1, double sigmaImage = 0.5);

    static Pyramid BuildDOG(Pyramid &pyramid);

    vector<FeaturePoint> getBlobFeaturePoints();

    double findPixel(int x, int y, float sigma);
};
