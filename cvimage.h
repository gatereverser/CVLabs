#pragma once

#include <QImage>
#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include<math.h>


using namespace std;

enum class BorderWrappingType{
    ZeroBorder,
    CopyBorder,
    ReflectBorder,
    WrapupBorder
};


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

  // CVImage& operator=(CVImage& other) = default;

    static CVImage fromFile(const QString &fileName);
    static CVImage fromQImage(const QImage &qImage);


    QImage toQImage();


    inline double getPixel(int testI, int testJ, BorderWrappingType type = BorderWrappingType::ZeroBorder) const{

        if(testI < height && testJ < width && testI >= 0 && testJ >= 0){
            return data[testI*width + testJ];

        }
        else{
            switch(type)
            {
                case BorderWrappingType::ZeroBorder:
                    return 0;
                    break;
                case BorderWrappingType::CopyBorder:
                    return data[min(max(testI,0), height -1) * width + min(max(testJ,0), width-1)];
                    break;
                case BorderWrappingType::ReflectBorder:
                    if(testI < 0) testI = testI * (-1) % height;
                    if(testJ < 0) testJ = testJ * (-1) % width;
                    if(testI >= height) testI =  testI % height;
                    if(testJ >= width) testJ = testJ % width;
                    return data[testI*width + testJ];
                    break;
                case BorderWrappingType::WrapupBorder:

                    if (testI < 0)
                        testI = height + testI % height;
                    if (testI >= height)
                        testI = testI % height;

                    if (testJ < 0)
                        testJ  = width + testJ % width;
                    if (testJ  >= width)
                        testJ  = testJ  % width;

                    return data[testI*width + testJ];
            }
        }

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




