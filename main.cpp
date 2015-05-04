#include "mainwindow.h"
#include <QApplication>
#include<QDebug>
#include"cv.h"
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




    CVImage image(CVImage::fromFile("image.png"));
    image.save("source.png");


    CVImage evilImage(CVImage::fromFile("15.jpg"));
    evilImage.save("source2.png");

    //image.downscale(2);
    //image.save("resized.png");
//    CVImage t(image.getHeight(),image.getWidth());
//    GaussSeparate(image,t,6, BorderWrappingType::CopyBorder);
//    t.save("gaussed.png");

///LAB5
    Pyramid pyr(Pyramid::Build(image,3,4,1.6));
    //Pyramid DOG(Pyramid::BuildDOG(pyr));
    //CVSobel(DOG.images[4],DOG.images[1]);
   // pyr.save("sem");
    vector<FeaturePoint> points3 = pyr.getBlobFeaturePoints();


    Pyramid pyr2(Pyramid::Build(evilImage,3,4,1.6));
    //Pyramid DOG(Pyramid::BuildDOG(pyr));
    //CVSobel(DOG.images[4],DOG.images[1]);
    //pyr.save("sem");
//    vector<FeaturePoint> points4 = pyr2.getBlobFeaturePoints();


//    CVImage descriptors3 (pyr.getSimpleDescriptors(points3));
//    CVImage descriptors4(pyr2.getSimpleDescriptors(points4));



//    vector<Dmatch> matches = matchDescriptors(descriptors3, descriptors4);

    pyr.pointsOutput(points3);
//    pyr2.pointsOutput(points4);
//    QImage matching = drawMatches(image, evilImage, points3, points4, matches);

//    matching.save("Matching.png");

    QImage har(image.toQImage());
    drawBlobs(har, points3);
    har.save("blobs.png");


///


//    ///LAB4



//    CVImage f1(image.getHeight(),image.getWidth());
//    GaussSeparate(image, f1, 0.3);
//    vector<FeaturePoint> points1 = harris(f1, 3, 10000);// Maybe 0.001?
//    CVImage descriptors1(getSimpleDescriptors(f1, points1));



//    CVImage f2(evilImage.getHeight(),evilImage.getWidth());
//    GaussSeparate(evilImage, f2, 0.3);
//    vector<FeaturePoint> points2 = harris(f2, 3, 10000);// Maybe 0.001?
//    CVImage descriptors2(getSimpleDescriptors(f2, points2));


//    vector<Dmatch> matches = matchDescriptors(descriptors1, descriptors2);

//    QImage matching = drawMatches(image, evilImage, points1, points2, matches);

//    matching.save("harris.png");


//    /// END OF LAB4


//    ////LAB 3

//    vector<FeaturePoint> points = moravec(image, 2, 3);
//    QImage moravec(image.toQImage());
//    drawPoints(moravec, points);
//    moravec.save("moravec.png");


//    CVImage u(image.getHeight(),image.getWidth());
//    GaussSeparate(image, u, 0.3);
//    vector<FeaturePoint> points2 = harris(u, 3, 2000);// Maybe 0.001?
//    getSimpleDescriptors(image, points2);
//    QImage har(image.toQImage());
//    drawPoints(har, points2);
//    har.save("harris.png");


//    /// END OF LAB3




//    ////LAB 2

//Pyramid pyr(Pyramid::Build(image,3,6,1.6));
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
