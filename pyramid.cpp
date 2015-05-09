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
        Pyramid result(octaveNum, levelNum + 3);
        double k = pow(2, 1./levelNum);// sigma0Start* - for future optimization

        double initialSigma = max(sigma0Start / k , sigmaImage);

        CVImage initial(image.getHeight(),image.getWidth());

        GaussSeparate(image, initial,sqrt(initialSigma*initialSigma - sigmaImage * sigmaImage),BorderWrappingType::ReflectBorder);

        result.images.push_back(initial);
        result.currentSigma.push_back(initialSigma);
        result.realSigma.push_back(initialSigma);
        result.currentOctave.push_back(0);
 cerr<<initialSigma<<endl;

        GaussSeparate(image, initial,sqrt(sigma0Start*sigma0Start - sigmaImage * sigmaImage),BorderWrappingType::ReflectBorder);

        result.images.push_back(initial);
        result.currentSigma.push_back(sigma0Start);
        result.realSigma.push_back(sigma0Start);
        result.currentOctave.push_back(0);

         cerr<<sigma0Start<<endl;

        for(int i=0; i<octaveNum; i++)
        {
            double octaveSigma = pow(2,i) * sigma0Start;
            for(int j=2; j < levelNum + 3; j++)
            {

                CVImage next(initial.getHeight(), initial.getWidth());
                GaussSeparate(initial, next,sqrt( sigma0Start* pow(k,j - 1) * sigma0Start* pow(k,j - 1) - sigma0Start * pow(k, j - 2) * sigma0Start * pow(k, j - 2)),BorderWrappingType::ReflectBorder);

                initial = next;

                result.images.push_back(next);                
                result.currentSigma.push_back(sigma0Start* pow(k,j - 1));
                result.realSigma.push_back(octaveSigma * pow(k,j - 1));
                result.currentOctave.push_back(i);
                cerr<<sigma0Start * pow(k, j - 1)<<" "<<octaveSigma * pow(k,j - 1)<<endl;

            }

            if( i != (octaveNum -1)){
                initial = result.images[i * (levelNum +3) + levelNum].downscale(2);
                result.images.push_back(initial);
                result.currentSigma.push_back(sigma0Start / k);
                result.realSigma.push_back(octaveSigma * 2 / k);
                result.currentOctave.push_back(i + 1);

                 cerr<<sigma0Start / k<<" "<<octaveSigma * 2 / k<<endl;


                initial = result.images[i * (levelNum +3) + levelNum + 1].downscale(2);
                result.images.push_back(initial);
                result.currentSigma.push_back(sigma0Start);
                result.realSigma.push_back(octaveSigma * 2);
                result.currentOctave.push_back(i + 1);

                cerr<<sigma0Start <<" "<<octaveSigma * 2<<endl;
            }



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
//double max = -50;
    for(int i=0; i < pyramid.images.size() - 1; i++)
    {
        if(pyramid.currentOctave[i] == pyramid.currentOctave[i + 1])
        {
            CVImage next(pyramid.images[i].getHeight(), pyramid.images[i].getWidth());
            for(int u = 0; u < next.getHeight(); u++){
                for(int v = 0; v < next.getWidth(); v++){
                    next.setPixel(u, v, pyramid.images[i + 1].getPixel(u,v) - pyramid.images[i].getPixel(u,v));
//                    if(pyramid.images[i + 1].getPixel(u,v) - pyramid.images[i].getPixel(u,v) > max){
//                        max = pyramid.images[i + 1].getPixel(u,v) - pyramid.images[i].getPixel(u,v);
//                    }
                }
            }
            result.images.push_back(next);
            result.currentSigma.push_back(pyramid.currentSigma[i]);
            result.realSigma.push_back(pyramid.realSigma[i]);
            result.currentOctave.push_back(pyramid.currentOctave[i]);
        }
    }
//    cout<<max<<"SDGSDGSD"<<endl;

//    for(int i = 0;i < result.images.size();i++){
//        result.images[i].normalize(0,255);
//    }

    return result;
}

vector<FeaturePoint> Pyramid::getBlobFeaturePoints(){

    Pyramid dog(BuildDOG(*this));

    //dog.save("sem");


    //cout<<dog.images[0].getPixel(125,122)<<" ADOLF";
    vector<FeaturePoint> points;
    points.clear();

    for(int i = 0 ;i < octaveCount; i++){
        for(int j = 1; j < levelsCount - 2; j++){

            int current = i * (levelsCount - 1) + j;

            int n = dog.images[current].getHeight();
            int m = dog.images[current].getWidth();

            int borderY = n /20;
            int borderX = m /20;


            CVImage Ix(n, m);
            CVImage Iy(n, m);
            CVImage Ixx(n, m);
            CVImage Iyy(n, m);
            CVImage Ixy(n, m);


            //OR  CVSobelSeparateX(images[i * levelsCount + j], Ix);
            CVSobelSeparateX(dog.images[current], Ix);
            CVSobelSeparateY(dog.images[current], Iy);
            CVSobelSeparateY(Ix, Ixy);
            CVSobelSeparateX(Ix, Ixx);
            CVSobelSeparateY(Iy, Iyy);


//            vector<FeaturePoint> harpoints = harris(images[i * levelsCount + j] ,3, 1);

            for(int u = borderY; u < n - borderY;u++){
                for(int v = borderX; v < m - borderX;v++){

                       if(fabs(dog.images[current].getPixel(u, v)) < 0.01){// CONTRAST IF EVERYWHERE
                           continue;
                       }

                    bool isMaximum = true;
                    bool isMinimum = true;

                    for (int t = 0; t < 26; t++) {

                        if (dog.images[current].getPixel(u, v) <=
                                dog.images[current + dz3d[t]].getPixel(u + dx3d[t], v + dy3d[t])){
                            isMaximum = false;
                        }
//                        else{
//                            cout<< dog.images[current].getPixel(u, v) - dog.images[current + dz3d[t]].getPixel(i + dx3d[t], j + dy3d[t])<<endl;
//                        }
                        if (dog.images[current].getPixel(u, v) >=
                                dog.images[current + dz3d[t]].getPixel(u + dx3d[t], v + dy3d[t]) ){
                            isMinimum = false;
                        }
                        if (!isMaximum && !isMinimum){
                            break;
                        }
                    }

                    if(isMaximum  || isMinimum){ //CHANGE THIS GODDAMN CODE
//                        bool truth = false;
//                        for(int h = 0;(h < harpoints.size())&&!truth;h++){
//                            if(harpoints[h].getX() == u && harpoints[h].getY() == v){
//                                truth = true;
//                            }
//                        }
//                        if(!truth) continue;

                        double dxx = Ixx.getPixel(u, v),
                             dxy = Ixy.getPixel(u, v),
                             dyy = Iyy.getPixel(u, v),
                             trace  = dxx + dyy,
                             determinat = dxx * dyy - dxy * dxy;

                        double edginess = trace * trace / determinat;
                        if (edginess > 12.1)
                            continue;
   // if(edginess< -200) cout<< u<<" "<<v<<" "<<currentOctave[i * (levelsCount) + j]<<endl;
                        auto dx = Ix.getPixel(u, v), dy = Iy.getPixel(u, v),
                             di = -(  dx * dyy - dy * dxy) / determinat,
                             dj = -(- dx * dxy + dy * dxx) / determinat;
                        if (fabs(dog.images[current].getPixel(u, v) - 0.5 * (dx * di + dy * dj)) < 8)
                        continue;

                       // if( realSigma[i * (levelsCount) + j] > 4.8)
                     points.emplace_back(u , v , realSigma[i * (levelsCount) + j], i * (levelsCount) + j);
                        // *pow(2, currentOctave[i * (levelsCount) + j])
                        //cout<<realSigma[i * (levelsCount) + j]<<endl;
                    }


                }
            }

        }
    }
     cout<<points.size()<< " POINTS"<<endl;

    return points;
}


CVImage  Pyramid::getSimpleDescriptors( vector<FeaturePoint> points, int binCount, int histCount, int cellCount ){


    int size = histCount / 2;


    CVImage detectors(points.size(), histCount * histCount * binCount);

    int cellInHistCount = cellCount / histCount;





    vector<CVImage> magnitudes;
    vector<CVImage> angles;
    for(int i = 0; i < images.size();i++)
    {

        int n = images[i].getHeight();
        int m = images[i].getWidth();
        CVImage Ix(n, m);
        CVImage Iy(n, m);

        CVSobelSeparateX(images[i], Ix);
        CVSobelSeparateY(images[i], Iy);

        CVImage magnitude(n, m);
        CVImage angle(n, m);
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++) {
                double valueX = Ix.getPixel(i, j);
                double valueY = Iy.getPixel(i,j);

                magnitude.setPixel(i, j, sqrt(valueX * valueX + valueY * valueY));
                if( atan2(valueY,valueX) * 180 /PII + 180 >= 360 || atan2(valueY,valueX) * 180 /PII + 180 < 0){
                    angle.setPixel(i, j, 359);
                }
                else{
                    angle.setPixel(i, j, atan2(valueY,valueX) * 180 /PII + 180);
                }
            }
        }

        magnitudes.emplace_back(magnitude);
        angles.emplace_back(angle);
    }

    //if(points.size() == 0) return detectors;

    for(int k = 0; k < (points.size() /* - 1*/);k++){


        double sigma = currentSigma[points[k].getLevel()] * cellCount / 2; // * size /  3;
        double denominator = (2 * 3.14 * sigma * sigma);
        double degreeDenominator = 2 * sigma * sigma;

        int posX = points[k].getX();
        int posY = points[k].getY();

        int n = images[points[k].getLevel()].getHeight();
        int m = images[points[k].getLevel()].getWidth();

        //REINFROCE
        int halfSize = cellCount / 2 * currentSigma[points[k].getLevel()];;

        //Calcaulating ANgle for a point
//        int angleHalfSize = cellCount / 2;
        const int numOrientation = 36;
        int rang = 360 / numOrientation;
        int dang = 360 / binCount;
        double angleOrientation[numOrientation];
        memset(angleOrientation, 0, numOrientation * sizeof(double));

        for (int i = posX - halfSize; i < posX + halfSize; i++){
            for (int j = posY - halfSize; j < posY + halfSize; j++) {

                double x = i - posX;
                double y = j - posY;
                double gausWeight = exp(-(x * x + y * y) / degreeDenominator) / denominator;

                double angle = angles[points[k].getLevel()].getPixel(i,j);                
                double magnitude = magnitudes[points[k].getLevel()].getPixel(i,j);

                double binNum  = angle / rang;
                int divedBinNum = (int) binNum;

                double whereToGo = (angle - (divedBinNum * rang +( divedBinNum +1)* rang) / 2);
                int direction = whereToGo < 0 ? (divedBinNum == 0 ? numOrientation - 1 : -1) : 1;

                double binFactor =  1 - (fabs(whereToGo)) / rang;

                angleOrientation[divedBinNum] += binFactor * gausWeight;
                angleOrientation[(divedBinNum + direction) % numOrientation ]+= (1 - binFactor);// * gausWeight;
            }
        }

//        for(int i = 0;i < 36;i++){
//            cout<<angleOrientation[i]<<endl;
//        }


        //(max_element(bin, bin + numOrientation) - bin)
        int maxIndex =  max_element(angleOrientation, angleOrientation + numOrientation) - angleOrientation;

//cout<<"GOTCHA "<<maxIndex * rang<<endl;
        points[k].setOrientation(maxIndex * rang);


//         double SUPERSUM = 0;
//        cout<<"HO LOL "<< points[k].getOrientation() << endl;
//        cout<< cos(points[k].getOrientation() * PII /180)<< " "<< sin(points[k].getOrientation() * PII /180)<<endl;

        double sqrt2 = sqrt(2);
        for(int i = posX - halfSize * sqrt2;i < posX + halfSize * sqrt2; i++){
            for(int j = posY - halfSize * sqrt2;j < posY + halfSize * sqrt2; j++){

double angle = angles[points[k].getLevel()].getPixel(i,j) - points[k].getOrientation();
                //gauss shift
                double tempX = i - posX;
                double tempY = j - posY;


                double x = tempX * cos(angle * PII /180) + tempY * sin(angle * PII /180);
                 double y =  - tempX * sin(angle * PII /180) + tempY * cos(angle * PII /180);


//                 double x = i - posX;
//                 double y = j - posY;


                if(angle < 0) angle += 360;
                double magnitude = magnitudes[points[k].getLevel()].getPixel(i,j);


                double binNum  = angle / dang;
                int divedBinNum = (int) binNum;

                double whereToGo = (angle - (divedBinNum * dang +( divedBinNum +1)* dang) / 2);
                int direction = whereToGo < 0 ? (divedBinNum == 0 ? binCount - 1 : -1) : 1;
                //cout<<"WHERHE "<< whereToGo<<endl;
                double binFactor =  1 - (fabs(whereToGo)) / dang;

                //consider in what hist it belongs

                if((x + halfSize)/ (halfSize / 2) < 0 || (x + halfSize)/ (halfSize / 2) > (histCount -1) ||
                   (y + halfSize)/ (halfSize / 2) < 0 || (y + halfSize)/ (halfSize / 2) > (histCount -1) ){
                    continue;
                }

                int binX = (x + halfSize)/ (halfSize / 2);
                int binY = (y + halfSize)/ (halfSize / 2);

                //cout<< binX<<" "<<binY<<endl;

                int row = binX * histCount * binCount;
                int column = binY * binCount;

                double gausWeight = exp(-(x * x + y * y) / degreeDenominator) / denominator;
                //cout<<gausWeight<<endl;


                //INTERPOLATION (ACtually not)
                detectors.setPixel(k,  row + column + divedBinNum % binCount,
                                   detectors.getPixel(k,  row + column + divedBinNum % binCount) + magnitude * gausWeight * binFactor);
                detectors.setPixel(k,  row + column + (divedBinNum + direction) % binCount,
                                   detectors.getPixel(k,  row + column + (divedBinNum + direction) % binCount) + magnitude * gausWeight * (1 - binFactor));

                //cout<< angle<<" "<<magnitude * gausWeight<<" "<< magnitude * gausWeight * binFactor<< " "<<magnitude * gausWeight * (1 - binFactor)<<endl;


//                 SUPERSUM += gausWeight;
            }
        }


//        if(SUPERSUM > 3)
//        cout<<SUPERSUM<<endl;

//        for(int i = 0; i < 128;i++){
//            cout<< detectors.getPixel(k,i)<< endl;
//        }
//        cout<<endl;



    }

    ///NORMALIZE HERE
    for(int i = 0;i < detectors.getHeight();i++){
        for(int k = 0;k < 2;k++){
            double unit = 0 ;
            for( int j = 0; j < detectors.getWidth(); j++){
                unit += detectors.getPixel(i, j) * detectors.getPixel(i, j);
            }

            unit = sqrt(unit);

            for( int j = 0; j < detectors.getWidth(); j++){
                detectors.setPixel(i, j, detectors.getPixel(i, j) / unit);
                if(detectors.getPixel(i, j) > 0.2 && !k)  detectors.setPixel(i, j, 0.2);
            }
        }

    }


    return detectors;
}

void Pyramid::pointsOutput(vector<FeaturePoint> &points){

    for(int k = 0;k < points.size();k++){
        points[k].setX(points[k].getX()* pow(2, currentOctave[points[k].getLevel()]));
        points[k].setY(points[k].getY()* pow(2, currentOctave[points[k].getLevel()]));
    }
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



