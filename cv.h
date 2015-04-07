#pragma once

#include "cvimage.h"
#include "cvkernel.h"
#include "featurepoint.h"



const int dx[] = {-1, -1, -1,  0, 0,  1, 1, 1};
const int dy[] = {-1,  0,  1, -1, 1, -1, 0, 1};




void Convolute(const CVImage &source, CVImage &dest, CVKernel &kernel, BorderWrappingType type = BorderWrappingType::ZeroBorder);

void CVSobelX(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelSeparateX(const CVImage source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelY(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelSeparateY(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobel(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder);
void GaussSeparate(const CVImage &source, CVImage &dest, double sigma, BorderWrappingType type = BorderWrappingType::ZeroBorder);

vector<FeaturePoint> findLocalMaximum(const CVImage& nonFilteredPoints, int threshold, int surroundingsHalfSize = 5);
vector<FeaturePoint> moravec(const CVImage &source, int windowHalfSize, int threshold, BorderWrappingType type = BorderWrappingType::ZeroBorder);
vector<FeaturePoint> nonMaximumSuppression(const vector<FeaturePoint> &nonSuppressedPoints,  int count = 500, int stepCount = 5000, double weightFactor = 0.9);


void drawPoints(QImage &image, const vector<FeaturePoint> points);
