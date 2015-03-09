#ifndef CVIMAGE
#define CVIMAGE

#include <QImage>
#include <iostream>
#include <memory>


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
    ~CVImage();

    static CVImage fromFile(const QString fileName);
    static CVImage fromQImage(const QImage qImage);


    QImage toQImage();


     double getPixel(int i, int j);
     void setPixel(int i, int j, double value);

     int getHeight();
     int getWidth();

     void save(const QString fileName);
     void normalize(double newMin, double newMax);
     void normalize(double oldMin, double oldMax, double newMin, double newMax);

};


#endif // CVIMAGE

