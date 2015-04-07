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


void CVSobelSeparateX(const CVImage source, CVImage &dest, BorderWrappingType type, bool isNorm){

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
    dest.normalize(0,255);

}


//LAB3 utils
vector<FeaturePoint> findLocalMaximum(const CVImage& nonFilteredPoints, int threshold, int surroundingsHalfSize){
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

vector<FeaturePoint> moravec(const CVImage &source, int windowHalfSize, int threshold, BorderWrappingType type){

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
  //  return moravecPoints;
    return nonMaximumSuppression(moravecPoints);
}


vector<FeaturePoint> nonMaximumSuppression(const vector<FeaturePoint> &nonSuppressedPoints,  int count, int stepCount, double weightFactor)
{
    vector<FeaturePoint> points(nonSuppressedPoints);


    double maxDistance = 0;

    for (int i = 0; i < points.size(); i++){

        for (int j = 0; j < points.size(); j++){
           double currentDistance = points[i].getDistance(points[j]);

            maxDistance = std::max(maxDistance, currentDistance);
        }

    }

    double step = maxDistance / stepCount;
    double radius = step;
    while (points.size() > count && radius <= maxDistance) {
        cout<<radius<<endl;
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



//COMMON HELPERS
void drawPoints(QImage &image, const vector<FeaturePoint> points){

    for (FeaturePoint point : points) {

        image.setPixel(point.getY(), point.getX(), point.getWei() << 16);
    }
}
