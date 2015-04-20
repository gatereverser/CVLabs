#include "cvimage.h"

CVImage::CVImage()
{
    data = nullptr;
    height = 0;
    width = 0;
}

CVImage::CVImage(int h, int w)
{
    data = make_unique<double[]>(h*w);
    height = h;
    width = w;
}

CVImage::CVImage(const CVImage &anotherImage)
{
    height = anotherImage.height;
    width= anotherImage.width;

    data = std::make_unique<double[]>(height*width);
    for (int i = 0; i < height*width; ++i)
        data[i] = anotherImage.data[i];
}

CVImage::CVImage(CVImage &&other)
{
    height = other.height;
    width = other.width;
    data = move(other.data);
//    data = std::make_unique<double[]>(height*width);
//    for (int i = 0; i < height*width; ++i)
//        data[i] = other.data[i];



    other.data = nullptr;
    other.height = 0;
    other.width = 0;
}

CVImage& CVImage::operator=(CVImage&& other) // move assignment
{
    if (this == &other) {
        return *this;
    }

    height = other.height;
    width = other.width;
    data = move(other.data);
    other.data = nullptr;
    other.height = 0;
    other.width = 0;
    return *this;
}

CVImage& CVImage::operator=(CVImage& other) // move assignment
{
    if (this == &other) {
        return *this;
    }

    height = other.height;
    width = other.width;

    data = std::make_unique<double[]>(height*width);
    for (int i = 0; i < height*width; ++i)
        data[i] = other.data[i];


    return *this;



}



CVImage::~ CVImage()
{

}

 CVImage  CVImage::fromFile(const QString &fileName)
{
    QImage qImage;
    bool isSuccessfull = qImage.load(fileName);
    if(isSuccessfull)
    {

        return fromQImage(qImage);
    }
    else
    {
        std::cerr << "can't load image from file";
        return  CVImage(0,0);
    }
}

 CVImage  CVImage::fromQImage(const QImage &qImage)
{
    int width = qImage.width();
    int height = qImage.height();
    CVImage image(height, width);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            double color = qImage.pixel(j, i);
            double r = qRed(color);
            double g = qGreen(color);
            double b = qBlue(color);

            double value = qRound(0.299 * r + 0.587 * g + 0.114 * b);

            image.setPixel(i, j, value);
        }
    }    
    return image;
}


QImage  CVImage::toQImage()
{
    QImage result = QImage(width, height, QImage::Format_RGB32);

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            int color = getPixel(i, j);
            result.setPixel(j, i, qRgb(color,color,color));
        }

    }

    return result;
}

void CVImage::save(const QString fileName)
{
    toQImage().save(fileName);
}

void CVImage::normalize(double newMin, double newMax)
{
    auto minmax = minmax_element(&data[0], &data[height * width]);
    double oldMin = *minmax.first;
    double oldMax = *minmax.second;


    for_each(&data[0], &data[height * width], [&] (double &value) {
        value = newMin + (newMax - newMin)*(value - oldMin)/(oldMax - oldMin);
     });

}

CVImage CVImage::downscale(int size)
{
    unique_ptr<double[]> result = make_unique<double[]>(height/size * width/size);

    for(int i=0; i< height - 1; i+=size)
    {
        // cout<<i<<endl;
        for(int j=0; j< width - 1; j+=size)
        {

            result[i*width / (size*size) + j/size] = getPixel(i,j);
        }

    }

    CVImage resultImage(height/size, width/size);
    resultImage.data = move(result);
    return resultImage;
}


void CVImage::normalize(double oldMin, double oldMax, double newMin, double newMax)
{
    for_each(&data[0], &data[height * width - 1], [&oldMin,&oldMax,&newMin,&newMax] (double &value) {
        value = newMin + (newMax - newMin)*(value - oldMin)/(oldMax - oldMin);
     });
}

