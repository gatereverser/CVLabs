#include "mainwindow.h"
#include <QApplication>
#include<Qimage>
#include<QDebug>
#include"cv.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();


    CVImage image(CVImage::fromFile("3.jpg"));
    image.save("source.png");


    CVImage s(image.getHeight(),image.getWidth());

    CVSobelX(image, s, BorderWrappingType::ZeroBorder, true);
    s.save("sobeledXZero.png");
    CVSobelX(image, s, BorderWrappingType::CopyBorder, true);
    s.save("sobeledXcopy.png");
    CVSobelX(image, s, BorderWrappingType::WrapupBorder, true);
    s.save("sobeledXwrap.png");
    CVSobelX(image, s, BorderWrappingType::ReflectBorder, true);
    s.save("sobeledXReflected.png");


    CVSobelSeparateX(image, s, BorderWrappingType::ZeroBorder, true);
    s.save("sobeledSeparateX.png");


    CVSobelY(image, s,BorderWrappingType::ZeroBorder, true);
    s.save("sobeledY.png");

    CVSobelSeparateY(image, s,BorderWrappingType::ZeroBorder, true);
    s.save("sobeledSeparateY.png");

    CVSobel(image, s);
    s.save("sobel.png");


    return 0;

}
