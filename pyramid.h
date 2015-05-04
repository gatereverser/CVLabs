#pragma once
#include "cv.h"
#include "featurepoint.h"

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

    CVImage getSimpleDescriptors(vector<FeaturePoint> points, int binCount = 8, int histCount = 4, int cellCount = 16);

    void pointsOutput(vector<FeaturePoint> &points);


    double findPixel(int x, int y, float sigma);
};
