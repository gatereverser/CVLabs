#include"pyramid.h"

Pyramid::Pyramid(int octaves, int levels)
{
    octaveCount = octaves;
    levelsCount = levels;
}

Pyramid::Pyramid(const Pyramid &other)
{
    octaveCount = other.octaveCount;
    levelsCount = other.levelsCount;


    for (int i = 0; i < other.images.size(); i++){
        images.push_back(other.images[i]);
        currentSigma.push_back(other.currentSigma[i]);
        realSigma.push_back(other.realSigma[i]);
        currentOctave.push_back(other.currentOctave[i]);
    }
}

Pyramid::Pyramid(Pyramid &&other)
{
    octaveCount = other.octaveCount;
    levelsCount = other.levelsCount;

    for (int i = 0; i < other.images.size(); i++){

        images.push_back(other.images[i]);
        currentSigma.push_back(other.currentSigma[i]);
        realSigma.push_back(other.realSigma[i]);
        currentOctave.push_back(other.currentOctave[i]);
    }

    other.images.clear();
    other.realSigma.clear();
    other.currentSigma.clear();
    other.currentOctave.clear();

    other.octaveCount = 0;
    other.levelsCount =0;

}


void Pyramid::save(const QString &fileName)
{

    for(int i=0; i<images.size(); i++)
    {
        QString fileAdditionName = "Octave" + QString::number(i/levelsCount) + "Level" + QString::number(i%levelsCount);
        images[i].save(fileName +  fileAdditionName + ".jpg");
    }

}

Pyramid Pyramid::Build(const CVImage &image, int octaveNum, int levelNum, double sigma0Start, double sigmaImage)
{
    if(octaveNum > 0 && levelNum > 0)
    {
        Pyramid result(octaveNum, levelNum);

        CVImage initial(image.getHeight(),image.getWidth());
        GaussSeparate(image, initial,sqrt(sigma0Start*sigma0Start - sigmaImage*sigmaImage),BorderWrappingType::ReflectBorder);


        double k = sigma0Start *pow(2, 1./levelNum);
        for(int i=0; i<octaveNum; i++)
        {
            double octaveSigma = pow(2,i) * sigma0Start;
            for(int j=0; j<levelNum; j++)
            {

                CVImage next(initial.getHeight(), initial.getWidth());
                GaussSeparate(initial, next,sigma0Start* pow(k,j),BorderWrappingType::ReflectBorder);
                result.images.push_back(next);
                result.currentSigma.push_back(sigma0Start* pow(k,j));
                result.realSigma.push_back(octaveSigma * pow(k,j));
                result.currentOctave.push_back(i);

                cerr<<sigma0Start * pow(k,j)<<" "<<octaveSigma * pow(k,j)<<endl;
            }
            initial.downscale(2);
        }

        return result;
    }
    else
    {
        cerr<<"null level or octave number"<<endl;
        Pyramid(0,0);
    }
}

double Pyramid::findPixel(int x, int y, float sigma)
{
    if(sigma <= realSigma[0])
    {
        return images[0].getPixel(x,y);
    }

    int downscale;

    if(sigma >= realSigma.back())
    {
        downscale = currentOctave.back() * 2;
        if(downscale == 0) downscale = 1;
        images.back().getPixel(x / downscale, y / downscale);
    }

    int position = 1;
    while(sigma <= realSigma[position]){
        position++;
    }

    int requiredOctave = currentOctave[position];

    downscale = requiredOctave * 2;
    if(downscale == 0) downscale = 1;

    double fromSigma = realSigma[position - 1];
    double toSigma = realSigma[position];
    double fromPixel =  (position%levelsCount == 0) ? // if sigma is between octaves
                images[position].getPixel(x  * 2 / downscale, y * 2 / downscale) :
                images[position].getPixel(x / downscale, y / downscale);
    double toPixel = images[position].getPixel(x / downscale, y / downscale);

    return fromPixel + (toPixel - fromPixel)*(sigma - fromSigma) / (toSigma - fromSigma);
}



