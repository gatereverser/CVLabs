#include"pyramid.h"

Pyramid::Pyramid(int octaves, int levels)
{
    octaveCount = octaves;
    levelsCount = levels;
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


        double k = pow(2, 1./levelNum);// sigma0Start* - for future optimization
        for(int i=0; i<octaveNum; i++)
        {
            double octaveSigma = pow(2,i) * sigma0Start;
            for(int j=0; j<levelNum; j++)
            {

                CVImage next(initial.getHeight(), initial.getWidth());
                GaussSeparate(initial, next,sigma0Start* pow(k,j),BorderWrappingType::ReflectBorder);
                if(j == levelNum-1)initial = next;
                result.images.push_back(next);                
                result.currentSigma.push_back(sigma0Start* pow(k,j));
                result.realSigma.push_back(octaveSigma * pow(k,j));
                result.currentOctave.push_back(i);
                cerr<<sigma0Start * pow(k,j)<<" "<<octaveSigma * pow(k,j)<<endl;

            }

            initial = initial.downscale(2);
        }

        return result;
    }
    else
    {
        cerr<<"null level or octave number"<<endl;
        Pyramid(0,0);
    }
}

Pyramid Pyramid::BuildDOG(Pyramid &pyramid){
    Pyramid result(pyramid.octaveCount, pyramid.levelsCount - 1);

    for(int i=0; i < pyramid.images.size() - 1; i++)
    {
        if(pyramid.currentOctave[i] == pyramid.currentOctave[i + 1])
        {
            CVImage next(pyramid.images[i].getHeight(), pyramid.images[i].getWidth());
            for(int u = 0; u < next.getHeight(); u++){
                for(int v = 0; v < next.getWidth(); v++){
                    next.setPixel(u, v, pyramid.images[i + 1].getPixel(u,v) - pyramid.images[i].getPixel(u,v));
                }
            }
            result.images.push_back(next);
            result.currentSigma.push_back(pyramid.currentSigma[i]);
            result.realSigma.push_back(pyramid.realSigma[i]);
            result.currentOctave.push_back(pyramid.currentOctave[i]);
        }
    }

    return result;
}


double Pyramid::findPixel(int x, int y, float sigma)
{
    if(sigma <= realSigma[0])
    {
        return images[0].getPixel(x,y);
    }

    double downscale;

    if(sigma >= realSigma.back())
    {
        downscale = pow(2,currentOctave.back());
        if(downscale == 0) downscale = 1;
        return images.back().getPixel(x / downscale, y / downscale);
    }

    int position = 1;
    while(sigma <= realSigma[position]){
        position++;
    }

    int requiredOctave = currentOctave[position];


    downscale = pow(2, requiredOctave);
    if(downscale == 0) downscale = 1;

    int previousRequiredOctave = 0 ; //for the between octaves case
    double prevDownscale = 0;
    if(position - 1 != -1 ){
        previousRequiredOctave = currentOctave[position - 1];
        prevDownscale = pow(2, previousRequiredOctave);;
    }

    //DANGEROUS PLACE
    double fromSigma = realSigma[position - 1];
    double toSigma = realSigma[position];
    double fromPixel =  (position%levelsCount == 0) ? // if sigma is between octaves
                images[position].getPixel(x  / prevDownscale, y  / prevDownscale) :
                images[position - 1].getPixel(x / downscale, y / downscale);
    double toPixel = images[position].getPixel(x / downscale, y / downscale);

    return fromPixel + (toPixel - fromPixel)*(sigma - fromSigma) / (toSigma - fromSigma);
}



