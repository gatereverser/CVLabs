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

CVImage::~ CVImage()
{

}

 CVImage  CVImage::fromFile(const QString fileName)
{
    QImage qImage;
    bool isSuccessfull = qImage.load(fileName);
    if(isSuccessfull)
    {
        return fromQImage(qImage);
    }
    else
    {
        std:cerr << "can't load image from file";
        return  CVImage(0,0);
    }
}

 CVImage  CVImage::fromQImage(const QImage qImage)
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


double  CVImage::getPixel(int i, int j)
{
        return data[i*width + j];

}

void  CVImage::setPixel(int i, int j, double value)
{
        data[i*width + j] = value;

}

int CVImage::getHeight()
{
    return height;
}

int CVImage::getWidth()
{
    return width;
}


void CVImage::normalize(double newMin, double newMax)
{
    double oldMin = INT_MAX, oldMax = INT_MIN;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(data[i*width+j] <oldMin)
                oldMin = data[i*width+j];
            else
                if(data[i*width+j] > oldMax)
                    oldMax = data[i*width+j];
        }
    }

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
           data[i*width+j] = newMin + (newMax - newMin)*(data[i*width+j] - oldMin)/(oldMax - oldMin);
        }
    }
}


void CVImage::normalize(double oldMin, double oldMax, double newMin, double newMax)
{
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
           data[i*width+j] = newMin + (newMax - newMin)*(data[i*width+j] - oldMin)/(oldMax - oldMin);
        }
    }
}

