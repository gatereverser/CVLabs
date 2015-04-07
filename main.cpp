#include "mainwindow.h"
#include <QApplication>
#include<Qimage>
#include<QDebug>
#include"pyramid.h"
#include <ctime>

using namespace std;

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();


    clock_t begin = clock();




    CVImage image(CVImage::fromFile("5.tif"));
    image.save("source.png");
    //image.downscale(2);
    //image.save("resized.png");
//    CVImage t(image.getHeight(),image.getWidth());
//    GaussSeparate(image,t,6, BorderWrappingType::CopyBorder);
//    t.save("gaussed.png");



    ////LAB 3

    vector<FeaturePoint> points = moravec(image, 1, 3);
    QImage moravec(image.toQImage());
    drawPoints(moravec, points);
    moravec.save("moravec.png");


    /// END OF LAB3




//    ////LAB 2

//Pyramid pyr(Pyramid::Build(image,3,3,1.6));
//pyr.save("sem");
//cerr<<pyr.findPixel(125,150,0.6)<<endl;

//    /// END OF LAB2

// //// LAB 1

//    CVImage s(image.getHeight(),image.getWidth());

//    CVSobelX(image, s, BorderWrappingType::ZeroBorder, true);
//    s.save("sobeledXZero.png");
//    CVSobelX(image, s, BorderWrappingType::CopyBorder, true);
//    s.save("sobeledXcopy.png");
//    CVSobelX(image, s, BorderWrappingType::WrapupBorder, true);
//    s.save("sobeledXwrap.png");
//    CVSobelX(image, s, BorderWrappingType::ReflectBorder, true);
//    s.save("sobeledXReflected.png");


//    CVSobelSeparateX(image, s, BorderWrappingType::ZeroBorder, true);
//    s.save("sobeledSeparateX.png");


//    CVSobelY(image, s,BorderWrappingType::ZeroBorder, true);
//    s.save("sobeledY.png");

//    CVSobelSeparateY(image, s,BorderWrappingType::ZeroBorder, true);
//    s.save("sobeledSeparateY.png");

//    CVSobel(image, s);
//    s.save("sobel.png");
// ////END OF LAB 1




    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cerr<<elapsed_secs;

    return 0;

}
