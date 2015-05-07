#pragma once

#include "cvimage.h"
#include "cvkernel.h"
#include "featurepoint.h"
#include "dmatch.h"

#define PII 3.14159265358979323846

const int dx[] = {-1, -1, -1,  0, 0,  1, 1, 1};
const int dy[] = {-1,  0,  1, -1, 1, -1, 0, 1};

const int dx3d[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1};
const int dy3d[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1};
const int dz3d[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};

void Convolute(const CVImage &source, CVImage &dest, CVKernel &kernel, BorderWrappingType type = BorderWrappingType::ZeroBorder);

void CVSobelX(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelSeparateX(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelY(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelSeparateY(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobel(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder);
void GaussSeparate(const CVImage &source, CVImage &dest, double sigma, BorderWrappingType type = BorderWrappingType::ZeroBorder);

vector<FeaturePoint> findLocalMaximum(const CVImage& nonFilteredPoints, double threshold, int surroundingsHalfSize = 3);
vector<FeaturePoint> moravec(const CVImage &source, int windowHalfSize, double threshold, BorderWrappingType type = BorderWrappingType::ZeroBorder);
vector<FeaturePoint> harris(const CVImage &source, int windowHalfSize, double threshold, double k = 0.04,  BorderWrappingType type = BorderWrappingType::ZeroBorder);
vector<FeaturePoint> nonMaximumSuppression(const vector<FeaturePoint> &nonSuppressedPoints, double maxDistance,  int count = 500, int stepCount = 5000, double weightFactor = 0.9);


CVImage getSimpleDescriptors(const CVImage &source, vector<FeaturePoint> points, int binCount = 8, int histCount = 4, int cellCount = 16);
vector<Dmatch> matchDescriptors(const CVImage &descriptors1, const CVImage &descriptors2);


void drawPoints(QImage &image, const vector<FeaturePoint> points);
void drawBlobs(QImage &image, vector<FeaturePoint> points);
QImage drawMatches(const CVImage &first, CVImage &second, vector<FeaturePoint> points1, vector<FeaturePoint> points2, vector<Dmatch> matches);
