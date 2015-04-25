#include"cv.h"

void Convolute(const CVImage &source, CVImage &dest, CVKernel &kernel, BorderWrappingType type)
{

    if(source.getHeight() != dest.getHeight() || source.getWidth() != dest.getWidth()){
        std::cerr<<"Sizes don't match";
        return;
    }

    double pixel;

    for(int i=0; i<dest.getHeight(); i++)
    {
        for(int j=0; j<dest.getWidth(); j++)
        {
            pixel = 0;

            for(int u = 0; u < kernel.getRowCount(); u++)
            {
                for(int v = 0; v< kernel.getColumnCount(); v++)
                {
                    double adding = kernel.getValue(u,v);
                    int testI = i-(u-kernel.getCenterY());
                    int testJ = j-(v-kernel.getCenterX());
                    adding *= source.getPixel(testI, testJ);
                    pixel +=adding;
                }
            }
            dest.setPixel(i, j, pixel);
        }
    }
}

void CVSobelX(const CVImage &source, CVImage &dest, BorderWrappingType type, bool isNorm){

    if(source.getHeight() != dest.getHeight() || source.getWidth() != dest.getWidth()){
        std::cerr<<"Sizes don't match";
        return;
    }

    double data[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    CVKernel kernel(3,3,data);
    Convolute(source,dest,kernel, type);
    if(isNorm)
    dest.normalize(0,255);
}


void CVSobelSeparateX(const CVImage &source, CVImage &dest, BorderWrappingType type, bool isNorm){

    if(source.getHeight() != dest.getHeight() || source.getWidth() != dest.getWidth()){
        std::cerr<<"Sizes don't match";
        return;
    }

    double rowData[] = {-1, 0, 1};
    double columnData[] = {1, 2, 1};

    CVKernel kernel1 (1,3,rowData);
    CVKernel kernel2 (3,1,columnData);

    CVImage temp(source.getHeight(), source.getWidth());
    Convolute(source, temp, kernel1, type);
    Convolute(temp, dest, kernel2, type);
    if(isNorm)
    dest.normalize(0,255);
}


void CVSobelY(const CVImage &source, CVImage &dest, BorderWrappingType type, bool isNorm){

    if(source.getHeight() != dest.getHeight() || source.getWidth() != dest.getWidth()){
        std::cerr<<"Sizes don't match";
        return;
    }

    double data[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    CVKernel kernel(3, 3 ,data);
    Convolute(source, dest, kernel, type);
    if(isNorm)
    dest.normalize(0,255);
}


void CVSobelSeparateY(const CVImage &source, CVImage &dest, BorderWrappingType type, bool isNorm){

    if(source.getHeight() != dest.getHeight() || source.getWidth() != dest.getWidth()){
        std::cerr<<"Sizes don't match";
        return;
    }

    double rowData[] = {1, 2, 1};
    double columnData[] = {-1, 0, 1};

    CVKernel kernel1 (1,3,rowData);
    CVKernel kernel2 (3,1,columnData);

    CVImage temp(source.getHeight(), source.getWidth());
    Convolute(source, temp, kernel1, type);
    Convolute(temp, dest, kernel2, type);
    if(isNorm)
    dest.normalize(0,255);
}

void CVSobel(const CVImage &source, CVImage &dest, BorderWrappingType type){

    if(source.getHeight() != dest.getHeight() || source.getWidth() != dest.getWidth()){
        std::cerr<<"Sizes don't match";
        return;
    }

    CVImage gradX(source.getHeight(), source.getWidth());
    CVImage gradY(source.getHeight(), source.getWidth());
    CVSobelSeparateX(source, gradX, type);
    CVSobelSeparateY(source, gradY, type);


    for(int i=0; i<dest.getHeight(); i++)
    {
        for(int j=0; j<dest.getWidth(); j++)
        {
            double x = gradX.getPixel(i,j);
            double y = gradY.getPixel(i,j);
            dest.setPixel(i,j, qRound(sqrt(x*x + y*y)));            
        }
    }

    dest.normalize(0, 255);
}


void GaussSeparate(const CVImage &source, CVImage &dest, double sigma, BorderWrappingType type)
{
    int k = 3*sigma;
    int size = 2*k+1;
    unique_ptr<double[]> data = make_unique<double[]> (size);


    for(int i=0; i<size; i++)
    {
        int x = i - k;
        double value = exp(-(x*x)/(2*sigma*sigma))/(sqrt(2*3.14)*sigma);
        data[i] = value;        
    }


    CVKernel kernel1 (1,size,data.get());
    CVKernel kernel2 (size,1,data.get());

    CVImage temp(source.getHeight(), source.getWidth());
    Convolute(source, temp, kernel1, type);
    Convolute(temp, dest, kernel2, type);
   // dest.normalize(0,255);

}


//LAB3 utils
vector<FeaturePoint> findLocalMaximum(const CVImage& nonFilteredPoints, double threshold, int surroundingsHalfSize){
    vector<FeaturePoint> points;

    int n = nonFilteredPoints.getHeight();
    int m = nonFilteredPoints.getWidth();

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++) {
            const double value = nonFilteredPoints.getPixel(i, j);
            bool isLocalMaximum = value > threshold;
            for (int di = -surroundingsHalfSize; isLocalMaximum && di <= surroundingsHalfSize; di++){
                for (int dj = -surroundingsHalfSize; isLocalMaximum && dj <= surroundingsHalfSize ; dj++) {
                   // if(di==0 && dj==0) continue;
                    const int shiftedI = i + di;
                    const int shiftedJ = j + dj;
                     //IMPORTANT:  value >= or > ??
                    if (shiftedI >= 0 && shiftedJ >= 0 && shiftedI < n && shiftedJ < m)
                        isLocalMaximum = value >= nonFilteredPoints.getPixel(shiftedI, shiftedJ);

                }
            }

            if (isLocalMaximum){
                points.push_back(FeaturePoint(i, j, value));
            }

        }
    }

    return points;

}

vector<FeaturePoint> moravec(const CVImage &source, int windowHalfSize, double threshold, BorderWrappingType type){

    int n = source.getHeight();
    int m = source.getWidth();

    CVImage nonFilteredPoints(n, m);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++) {
            double minValue = INFINITY;
            for (int k = 0; k < 8; k++) {
                double currentValue = 0;
                for (int u = -windowHalfSize; u <= windowHalfSize; u++){
                    for (int v = -windowHalfSize; v <= windowHalfSize; v++){
                        double differnece = source.getPixel(i + u, j + v, type)
                                - source.getPixel(i + u + dx[k], j + v + dy[k], type);
                        currentValue += pow(differnece, 2);
                    }
                }
                minValue = std::min(minValue, currentValue);
            }
            nonFilteredPoints.setPixel(i, j, minValue);
        }
    }

    vector<FeaturePoint> moravecPoints  =  findLocalMaximum(nonFilteredPoints, threshold);
    // return moravecPoints;

    double maxDistance = n * n + m * m;
    return nonMaximumSuppression(moravecPoints, maxDistance);
}


vector<FeaturePoint> harris(const CVImage &source, int windowHalfSize, double threshold, double k, BorderWrappingType type){

    int n = source.getHeight();
    int m = source.getWidth();

    CVImage nonFilteredPoints(n, m);
    CVImage Ix(n, m);
    CVImage Iy(n, m);
    CVImage Ix2(n, m);
    CVImage Iy2(n, m);
    CVImage Ixy(n, m);

    CVSobelSeparateX(source, Ix);
    CVSobelSeparateY(source, Iy);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++) {
            double xValue = Ix.getPixel(i, j);
            double yValue = Iy.getPixel(i, j);
            Ix2.setPixel(i, j, xValue * xValue);
            Iy2.setPixel(i, j, yValue * yValue);
            Ixy.setPixel(i, j, xValue * yValue);
        }
    }

    double sigma = (double) windowHalfSize / 3;
    double denominator = (2 * 3.14 * sigma * sigma);
    double degreeDenominator = 2 * sigma * sigma;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++) {
            double a = 0, b = 0, c = 0;
            double weightedSum = 0;
            for (int u = -windowHalfSize; u <= windowHalfSize; u++){
                for (int v = -windowHalfSize; v <= windowHalfSize; v++){

                    int x = u ;
                    int y = v ;
                    double value = exp(-(x * x + y * y) / degreeDenominator) / denominator;


                    a += value * Ix2.getPixel(i + u, j + v);
                    b += value * Ixy.getPixel(i + u, j + v);
                    c += value * Iy2.getPixel(i + u, j + v);
                }
            }
            //if (i == 500)cout<< a<<" " <<Ix3.getPixel(i,j)<<endl;

            //STANDARD SUM
           // weightedSum = a * c - b * b - k * (a + b) * (a + b);

            //MIN LAMBDA SUM
            double d = sqrt((a - c) * (a - c) + 4 * b * b);

            double lambda1 = (a + c - d) / 2;
            double lambda2 = (a + c + d) / 2;

            double lambdaMin = std::min(fabs(lambda1), fabs(lambda2));

           //cout<<a<< " "<< b<< " "<< c << " "<<lambdaMin<<endl;
            nonFilteredPoints.setPixel(i, j, lambdaMin);
        }
    }

    vector<FeaturePoint> harrisPoints  =  findLocalMaximum(nonFilteredPoints, threshold);
    //return harrisPoints;

    double maxDistance = n * n + m * m;
    return nonMaximumSuppression(harrisPoints, maxDistance, 50);
}



vector<FeaturePoint> nonMaximumSuppression(const vector<FeaturePoint> &nonSuppressedPoints, double maxDistance,  int count, int stepCount, double weightFactor)
{
    vector<FeaturePoint> points(nonSuppressedPoints);

    double step = maxDistance / stepCount;
    double radius = step;
    while (points.size() > count && radius <= maxDistance) {
        // cout<<radius<<endl;
        for (int i = 0; i < points.size() && points.size() > count; i++) {
            bool strongest = true;

            for (int j = 0; j < points.size() && strongest; j++){
                if (i != j){
                    if (points[i].getDistance(points[j]) <= radius){
                        if (points[i].getWei() * weightFactor  < points[j].getWei()){
                            strongest = false;
                        }
                    }
                }
            }

            if (!strongest) {
                points.erase(points.begin() + i);
                i--;
            }
        }
        radius += step;
    }


    return points;
}


//LAB4
CVImage  getSimpleDescriptors(const CVImage &source, vector<FeaturePoint> points, int binCount, int histCount, int cellCount ){

    int n = source.getHeight();
    int m = source.getWidth();
    int size = histCount / 2;

    CVImage Ix(n, m);
    CVImage Iy(n, m);
    CVSobelSeparateX(source, Ix);
    CVSobelSeparateY(source, Iy);

    CVImage detectors(points.size(), histCount * histCount * binCount);
    int cellInHistCount = cellCount / histCount;

    double sigma = cellCount / 2; // * size /  3;
    double denominator = (2 * 3.14 * sigma * sigma);
    double degreeDenominator = 2 * sigma * sigma;
    double magicConst = 0.465644;

    //REINFROCE
    int halfSize = cellCount / 2;

    for(int k = 0; k < points.size();k++){
        // double SUPERSUM = 0;
        int posX = points[k].getX();
        int posY = points[k].getY();
        for(int i = posX - halfSize;i < posX + halfSize; i++){
            for(int j = posY - halfSize;j < posY + halfSize; j++){


                //gauss shift
                double x = i - posX;
                double y = j - posY;

                //getting shifted coordiantes
                double valueX = Ix.getPixel(i, j);
                double valueY = Iy.getPixel(i,j);
                double angle = atan2(valueY,valueX) * 180 /3.14 + 180;

               // cout<<angle<<endl;
                double magnitude = sqrt(valueX * valueX + valueY * valueY);

                int dang = 360 / binCount;
                double binNum  = angle / dang;
                int divedBinNum = (int) binNum;


                double binFactor =  1 - (angle - divedBinNum * dang) / dang;

                //consider in what hist it belongs
                int binX = (x + halfSize)/ (halfSize / 2);
                int binY = (y + halfSize)/ (halfSize / 2);

                //cout<< binX<<" "<<binY<<endl;

                int row = binX * histCount * binCount;
                int column = binY * binCount;

                double gausWeight = exp(-(x * x + y * y) / degreeDenominator) / denominator / magicConst;
//cout<<gausWeight<<endl;

                detectors.setPixel(k,  row + column + divedBinNum % 8,
                                   detectors.getPixel(k,  row + column + divedBinNum % 8) + magnitude * gausWeight * binFactor);
                detectors.setPixel(k,  row + column + (divedBinNum + 1) % 8,
                                   detectors.getPixel(k,  row + column + (divedBinNum + 1) % 8) + magnitude * gausWeight * (1 - binFactor));

                //cout<< angle<<" "<<magnitude * gausWeight<<" "<< magnitude * gausWeight * binFactor<< " "<<magnitude * gausWeight * (1 - binFactor)<<endl;


                // SUPERSUM += gausWeight;
               // cout<<magnitude * gausWeight<<endl;
            }
        }
//        cout<<SUPERSUM<<endl;
//        for(int i = 0; i < 128;i++){
//            cout<< detectors.getPixel(k,i)<< endl;
//        }
//        cout<<endl;
        ///NORMALIZE HERE
    }

    return detectors;
}


vector<Dmatch> matchDescriptors(const CVImage &descriptors1, const CVImage &descriptors2){

    vector<Dmatch> answer;
    if( descriptors1.getWidth() != descriptors2.getWidth()){
        cout<<"Descriptors can't be matched"<<endl;
        return answer;
    }
    int height1 = descriptors1.getHeight();
    int height2 = descriptors2.getHeight();
    int m = descriptors2.getWidth();
    const double identityFactor = 0.8;

    for(int i = 0;i < height1; i++){

        double minDistance = INFINITY;
        double secondMinDistance = INFINITY;
        int minNumber = -1;
        int secondMinNumer = -1;
        for(int j = 0; j < height2; j++){

            double currentDistance = 0;
            for(int k = 0; k < m; k++){

                currentDistance += (descriptors1.getPixel(i,k) - descriptors2.getPixel(j, k)) *
                        (descriptors1.getPixel(i,k) - descriptors2.getPixel(j, k));
            }
            if(currentDistance < minDistance){

                secondMinDistance = minDistance;
                secondMinNumer = minNumber;
                minDistance = currentDistance;
                minNumber = j;
            }
        }
        //cout<<minDistance<<endl;

        //INSERT RATIO CHECK
        answer.push_back(Dmatch(i, minNumber, minDistance));
        double ratioDistance = minDistance / secondMinDistance;
        if(ratioDistance < identityFactor){
           // answer.push_back(Dmatch(i, secondMinNumer, secondMinDistance));
        }
    }


    return answer;
}



//COMMON HELPERS
void drawPoints(QImage &image, const vector<FeaturePoint> points){

    const int di[] = {0, 0, 1, -1};
    const int dj[] = {-1, 1, 0, 0};

    for (FeaturePoint point : points) {

        image.setPixel(point.getY(), point.getX(), point.getWei() << 16);
        for(int i = 0;i < 4;i++){
            if(point.getX() + di[i] >= 0 && point.getY() + dj[i] >= 0 && point.getY() + dj[i] < image.width() && point.getX() + di[i] < image.height() ){
                 image.setPixel(point.getY() + dj[i], point.getX() + di[i], /*point.getWei()*/ 255 << 16);
            }
        }
    }
}

QImage drawMatches(const CVImage &first, CVImage &second, vector<FeaturePoint> points1, vector<FeaturePoint> points2, vector<Dmatch> matches){
    int maxHeight = max(first.getHeight(), second. getHeight());

     QImage final = QImage(first.getWidth() + second.getWidth(), maxHeight, QImage::Format_RGB32);


     //Making doubleImage
     for(int i=0; i<first.getHeight(); i++)
     {
         for(int j=0; j<first.getWidth(); j++)
         {
             int color = first.getPixel(i, j);
             final.setPixel(j, i, qRgb(color,color,color));
         }

     }

     for(int i=0; i<second.getHeight(); i++)
     {
         for(int j=0; j<second.getWidth(); j++)
         {
             int color = second.getPixel(i, j);
             final.setPixel(j + first.getWidth(), i, qRgb(color,color,color));
         }

     }


     //Shifting Points
     for(int i = 0;i < points2.size(); i++){
         points2[i].setY(points2[i].getY() + first.getWidth());
     }

     drawPoints(final, points1);
     drawPoints(final, points2);

      QPainter p(&final);

        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::SquareCap));

        for(int i = 0;i < matches.size(); i++){
//cout<<matches[i].distance<<endl;
            p.drawLine(points1[matches[i].firstMatch].getY(), points1[matches[i].firstMatch].getX(), points2[matches[i].secondMatch].getY(), points2[matches[i].secondMatch].getX());

        }

         p.end(); // Don't forget this line!

     return final;
}
