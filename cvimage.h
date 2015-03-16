#pragma once

#include <QImage>
#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>


using namespace std;


class CVImage{
private:
    unique_ptr<double[]> data;
    int height;
    int width;
public:
    CVImage();
    CVImage(int h, int w);
    CVImage(const  CVImage &anotherImage);
    CVImage(CVImage&& other);
    ~CVImage();

    static CVImage fromFile(const QString &fileName);
    static CVImage fromQImage(const QImage &qImage);


    QImage toQImage();


     double getPixel(int i, int j) const;
     void setPixel(int i, int j, double value);

     int getHeight() const;
     int getWidth() const;

     void save(const QString fileName);
     void normalize(double newMin, double newMax);
     void normalize(double oldMin, double oldMax, double newMin, double newMax);
     void downscale(int size);

};




