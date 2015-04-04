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

    //DOn't forget to make operator = for copying
    CVImage& operator=(CVImage&& other);


    static CVImage fromFile(const QString &fileName);
    static CVImage fromQImage(const QImage &qImage);


    QImage toQImage();


     inline double getPixel(int i, int j) const{
         return data[i*width + j];

    }

     inline void setPixel(int i, int j, double value){
         data[i*width + j] = value;

     }

     inline int getHeight() const {
         return height;
     }

     inline int getWidth() const{
         return width;
     }

     void save(const QString fileName);
     void normalize(double newMin, double newMax);
     void normalize(double oldMin, double oldMax, double newMin, double newMax);

     CVImage downscale(int size);

};




