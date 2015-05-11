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

    //double sum  = 0;

    for(int i=0; i<size; i++)
    {
        int x = i - k;
        double value = exp(-(x*x)/(2*sigma*sigma))/(sqrt(2*3.14)*sigma);
        data[i] = value;
     //   sum+=value;
    }

   // cout<<sum<<" aFAS"<<endl;


    CVKernel kernel1 (1,size,data.get());
    CVKernel kernel2 (size,1,data.get());

    CVImage temp(source.getHeight(), source.getWidth());
    Convolute(source, temp, kernel1, type);
    Convolute(temp, dest, kernel2, type);
    //dest.normalize(0,255);

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

    for (int i = windowHalfSize; i < n - windowHalfSize; i++){
        for (int j = windowHalfSize; j < m - windowHalfSize; j++) {
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
    //cout<<harrisPoints.size()<<endl;
    return harrisPoints;

    double maxDistance = n * n + m * m;
    return nonMaximumSuppression(harrisPoints, maxDistance, 100);
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


    int size = histCount / 2;






    CVImage detectors(points.size(), histCount * histCount * binCount);
    int cellInHistCount = cellCount / histCount;

    double sigma = cellCount / 2; // * size /  3;
    double denominator = (2 * 3.14 * sigma * sigma);
    double degreeDenominator = 2 * sigma * sigma;
    double magicConst = 0.465644;

    //REINFROCE
    int halfSize = cellCount / 2 ;

    int n = source.getHeight();
    int m = source.getWidth();
    CVImage Ix(n, m);
    CVImage Iy(n, m);
    CVSobelSeparateX(source, Ix);
    CVSobelSeparateY(source, Iy);

    CVImage magnitudes(n, m);
    CVImage angles(n, m);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++) {
            double valueX = Ix.getPixel(i, j);
            double valueY = Iy.getPixel(i,j);

            magnitudes.setPixel(i, j, sqrt(valueX * valueX + valueY * valueY));
            angles.setPixel(i, j, atan2(valueY,valueX) * 180 /3.14 + 180);
        }
    }


    for(int k = 0; k < points.size();k++){


        // double SUPERSUM = 0;
        int posX = points[k].getX();
        int posY = points[k].getY();
        for(int i = posX - halfSize;i < posX + halfSize; i++){
            for(int j = posY - halfSize;j < posY + halfSize; j++){


                //gauss shift
                double x = i - posX;
                double y = j - posY;

                double angle = angles.getPixel(i,j);
                double magnitude = magnitudes.getPixel(i,j);

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

                detectors.setPixel(k,  row + column + divedBinNum % binCount,
                                   detectors.getPixel(k,  row + column + divedBinNum % binCount) + magnitude * gausWeight * binFactor);
                detectors.setPixel(k,  row + column + (divedBinNum + 1) % binCount,
                                   detectors.getPixel(k,  row + column + (divedBinNum + 1) % binCount) + magnitude * gausWeight * (1 - binFactor));

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


//LAB5



vector<Dmatch> matchDescriptors(const CVImage &descriptors1, const CVImage &descriptors2, vector<FeaturePoint> points1, vector<FeaturePoint> points2){

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
        //if(minDistance  < 0.003)

        if(secondMinNumer!=-1){
            double ratioDistance = minDistance / secondMinDistance;
            if((ratioDistance < identityFactor)){

                if(points1[i].getScale() < 5 * points2[minNumber].getScale() &&
                    points1[i].getScale() > 0.2 * points2[minNumber].getScale()){
                answer.emplace_back(i, minNumber, minDistance);
                }
              // answer._back(Dmatch(i, secondMinNumer, secondMinDistance));
            }
        }

    }


    return answer;
}


QImage drawMatches(const CVImage &first, const CVImage &second, vector<FeaturePoint> points1, vector<FeaturePoint> points2, vector<Dmatch> matches, int t[4]){
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
      // p.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::SquareCap));

        for(int i = 0;i < 4; i++){
cout<<matches[i].distance<< "ADOLF"<< endl;
            if(points1[matches[t[i]].firstMatch].getScale() < 5 * points2[matches[t[i]].secondMatch].getScale() &&
                points1[matches[t[i]].firstMatch].getScale() > 0.2 * points2[matches[t[i]].secondMatch].getScale()){
                p.setPen(QColor(abs(rand()) % 256, abs(rand()) % 256, abs(rand()) % 256));
                p.drawLine(points1[matches[t[i]].firstMatch].getY(), points1[matches[t[i]].firstMatch].getX(), points2[matches[t[i]].secondMatch].getY(), points2[matches[t[i]].secondMatch].getX());

                auto& p1 = points1[matches[t[i]].firstMatch];
                p.drawEllipse(QPointF(p1.getY(),p1.getX()), p1.getScale()* sqrt(2), p1.getScale()*sqrt(2));
                auto& p2 = points2[matches[t[i]].secondMatch];
                p.drawEllipse(QPointF(p2.getY(),p2.getX()), p2.getScale()* sqrt(2), p2.getScale()*sqrt(2));
            }

        }

         p.end(); // Don't forget this line!
         final.save("WATAFAFafgweK.png");
}


//LAB 8
void homography(const CVImage &from, const CVImage &to, vector<FeaturePoint> points1, vector<FeaturePoint> points2, vector<Dmatch> matches, double param[9], int iterationCount, double threshold){

    int bestCountInliers = 0;

//    Eigen::MatrixXd A(8, 8);
//    Eigen::VectorXd B(8), x(8);
//    Eigen::Matrix3d F, M;
//    Eigen::VectorXd C(4);


    double m[9];

    int bestbestmat[4];

    gsl_matrix* A = gsl_matrix_alloc(8,9);
    gsl_matrix* ATransposed = gsl_matrix_alloc(9,8);
    gsl_matrix* AtA = gsl_matrix_alloc(9,9);
    gsl_matrix* V = gsl_matrix_alloc(9,9);
    gsl_vector* S = gsl_vector_alloc(9);


    while(iterationCount > -1){
        iterationCount--;
        //if(iterationCount % 100 ==0) cout<<"CAVUM"<<endl;

        static unsigned int seed = 0;
        random_device rd;
        mt19937 mt((++seed) + time(NULL));


        uniform_int_distribution<int> rnd(0, matches.size() - 1);


        int chosenMatches[] = {rnd(mt), rnd(mt), rnd(mt), rnd(mt)};


        while(1){
            int i = 0;
            if((chosenMatches[i] == chosenMatches[i+1]) || (chosenMatches[i] == chosenMatches[i+1])||
                (chosenMatches[i] == chosenMatches[i+2]) || (chosenMatches[i] == chosenMatches[i+3])||
                (chosenMatches[i + 1] == chosenMatches[i+2]) || (chosenMatches[i + 1] == chosenMatches[i+3])||
                    (chosenMatches[i + 2] == chosenMatches[i+3])){
                for( i = 0; i < 4;i++){
                    chosenMatches[i]= rnd(mt);
                }
            }
            else{
                break;
            }
        }
        for(int i = 0; i < 3;i++){
          if(chosenMatches[i] == chosenMatches[i+1]){
              chosenMatches[i + 1] =  rnd(mt);
              i = 0;
          }
        }

    //FILL TE MATRIX NOW!!!!

        for(int i = 0;i < 4; i++){
//cout<< chosenMatches[i] <<"sfsd"<<endl;
//            cout<<matches.size()<< " " << points1.size()<< " "<< points2.size()<<endl;

            int x = points1[matches[chosenMatches[i]].firstMatch].getX();
//            cout<<x<<" ";
            int y = points1[matches[chosenMatches[i]].firstMatch].getY();
//cout<<y<<" ";
            int xd = points2[matches[chosenMatches[i]].secondMatch].getX();
//            cout<<xd<<" ";
            int yd = points2[matches[chosenMatches[i]].secondMatch].getY();
//            cout<<yd<<" ";

//            if(i==0){
//            x = 299;
//            y = 788;
//            xd = 178;
//            yd = 395;
//            }

//            if(i==1){
//            x = 340;
//            y = 1181;
//            xd = 231;
//            yd = 762;
//            }

//            if(i==2){
//            x = 887;
//            y = 1213;
//            xd = 702;
//            yd = 792;
//            }

//            if(i==3){
//            x = 908;
//            y = 526;
//            xd = 838;
//            yd = 151;
//            }

            gsl_matrix_set(A ,2 * i, 0, x);
            gsl_matrix_set(A ,2 * i, 1, y);
            gsl_matrix_set(A ,2 * i, 2, 1);
            gsl_matrix_set(A ,2 * i, 3, 0);
            gsl_matrix_set(A ,2 * i, 4, 0);
            gsl_matrix_set(A ,2 * i, 5, 0);
            gsl_matrix_set(A ,2 * i, 6, -xd * x);
            gsl_matrix_set(A ,2 * i, 7, -xd * y);
            gsl_matrix_set(A ,2 * i, 8, -xd);


            gsl_matrix_set(A ,2 * i + 1, 0, 0);
            gsl_matrix_set(A ,2 * i + 1, 1, 0);
            gsl_matrix_set(A ,2 * i + 1, 2, 0);
            gsl_matrix_set(A ,2 * i + 1, 3, x);
            gsl_matrix_set(A ,2 * i + 1, 4, y);
            gsl_matrix_set(A ,2 * i + 1, 5, 1);
            gsl_matrix_set(A ,2 * i + 1, 6, -yd * x);
            gsl_matrix_set(A ,2 * i + 1, 7, -yd * y);
            gsl_matrix_set(A ,2 * i + 1, 8, -yd);

        }

        //MATRIX FILLLED

        gsl_matrix_transpose_memcpy(ATransposed, A);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1, ATransposed, A, 0 , AtA);


//        for(int i = 0;i < 9 ;i ++){
//            for(int j = 0;j < 9 ;j++){
//            cout<<gsl_matrix_get(AtA, i ,j)<< " ";
//            }
//            cout<<endl;
//            //cout<<m[i]<<endl;
//        }


        gsl_linalg_SV_decomp_jacobi(AtA, V, S);


//        for(int i = 0;i < 9 ;i ++){
//            for(int j = 0;j < 9 ;j ++){

//                cout<< gsl_matrix_get(V, i ,j)<<" ";
//            }
//            cout<<endl;

//        }


        for(int i = 0;i < 9 ;i ++){
            m[i] = gsl_matrix_get(V, i ,8);
//            cout<<m[i]<<endl;
        }


//        int x = 50;
//        int y = 459;
//        xd = 50;
//        yd = 729;
//cout<<m[4] * y<< "DAFQA"<<endl;
//        int xCalculate = (m[0] * x + m[1] * y + m[2]) / (m[6] * x  + m[7] * y + m[8]);
//        int yCalcualte = (m[3] * x+ m[4] * y+ m[5]) / (m[6] * x  + m[7] * y + m[8]) ;


//        cout<<xCalculate<< " "<<yCalcualte<<endl;




        //cout<<matches[chosenMatches[0]].firstMatch<<endl;

        int cntInlier = 0;

        for(int i = 0;i < matches.size();i++){

            int xFrom = points1[matches[i].firstMatch].getX();

            int yFrom = points1[matches[i].firstMatch].getY();

            if(points1[matches[i].firstMatch].getScale() < 5 * points2[matches[i].secondMatch].getScale() &&
                points1[matches[i].firstMatch].getScale() > 0.2 * points2[matches[i].secondMatch].getScale()){
                int xTo = points2[matches[i].secondMatch].getX();

                int yTo = points2[matches[i].secondMatch].getY();

                int xCalculate = (m[0] * xFrom + m[1] * yFrom + m[2]) / (m[6] * xFrom  + m[7] * yFrom + m[8]);
                int yCalculate = (m[3] * xFrom + m[4] * yFrom + m[5]) / (m[6] * xFrom  + m[7] * yFrom + m[8]);

                double distance  = (xCalculate - xTo) * (xCalculate - xTo) + (yCalculate - yTo) * (yCalculate - yTo);
                if(distance <= threshold){
                    cntInlier++;
                }
            }

        }


        //cout<<cntInlier<<"COMMMON"<<endl;

        if(cntInlier > bestCountInliers){
            bestCountInliers = cntInlier;
            cout<< bestCountInliers<<" THEY ARE BEST AROUND"<<endl;
            for(int i = 0;i < 9;i++){
                param[i] = m[i];
            }

           //Which does he conenct
            for(int i = 0;i < 4; i++){
            bestbestmat[i] = chosenMatches[i];
            cout<<"I HATE YOU"<< bestbestmat[i]<<endl;
            }
        }


    }
    for(int i = 0;i < 9;i++){
        cout<<param[i]<<endl;
    }

   //drawMatches(from, to, points1, points2, matches, bestbestmat);



    cout<<"TESTING TIME!"<<endl;
    for(int i = 0;i < matches.size();i++){

        int xFrom = points1[matches[i].firstMatch].getX();
        int yFrom = points1[matches[i].firstMatch].getY();
        int xTo = points2[matches[i].secondMatch].getX();
//            cout<<xd<<" ";
        int yTo = points2[matches[i].secondMatch].getY();

        int xCalculate = (param[0] * xFrom + param[1] * yFrom + param[2]) / (param[6] * xFrom  + param[7] * yFrom + param[8]);
        int yCalculate = (param[3] * xFrom + param[4] * yFrom + param[5]) / (param[6] * xFrom  + param[7] * yFrom + param[8]);


        points2[matches[i].secondMatch].setX(xCalculate);
        points2[matches[i].secondMatch].setY(yCalculate);
        double distance  = (xCalculate - xTo) * (xCalculate - xTo) + (yCalculate - yTo) * (yCalculate - yTo);
       // cout<< distance<< "DISTANCE MAN" <<endl;
       // cout<<xFrom<< " "<< yFrom<< " "<< xCalculate<<" "<< yCalculate<<endl;

    }

    //drawMatches(from, to, points1, points2, matches);


}


//LAB9

void hough(const CVImage &descriptors1, const CVImage &descriptors2, vector<FeaturePoint> points1,
vector<FeaturePoint> points2, vector<Dmatch> matches, double param[9], int minX, int maxX, int minY, int maxY,
 double maxScale, double ds, double da, double dx , double dy){

    PhaseSpace ps(maxScale, ds, da, minX, maxX, dx, minY, maxY, dy);


    for(int i = 0;i < matches.size();i++){
        if(matches[i].distance<=0.3){
            int from = matches[i].firstMatch;
            int to = matches[i].secondMatch;
            double angle = points2[to].getOrientation() - points1[from].getOrientation();
            angle  *= PII;
            angle /= 180;
            while (angle < 0) angle += 2 * PII;
            double y = points1[from].getX();
            double x = points1[from].getY();
            double cosa = cos(angle), sina = sin(angle);
            double temp = x * cosa - y * sina;
            y = x * sina + y * cosa;
            x = temp;
            double m = points2[to].getScale() / points1[from].getScale();
            x *= m; y *= m;
            double dx = points2[to].getY() - x, dy =  points2[to].getX() - y;
            ps.add(m, angle, dx, dy, 1);
        }
    }

    double s, a, di, dj;
    ps.getMax(s, a, dj, di);

    qDebug() << "parameters: " << s << a << dj << di;

    auto cosa = cos(a), sina = sin(a);
    qDebug() << "sina =" << sina;
    param[0] = s * cosa;
    param[1] = s * sina;
    param[2] = di;
    param[3] = -s * sina;
    param[4] = s * cosa;
    param[5] = dj;







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


void drawBlobs(QImage &image, vector<FeaturePoint> points){

//    for (FeaturePoint point : points) {

//        image.setPixel(point.getY(), point.getX(), 255 << 16);

//    }

    QPainter p(&image);

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::SquareCap));

    for(int i = 0;i < points.size(); i++){

        p.drawEllipse(QPointF(points[i].getY(),points[i].getX()), points[i].getScale()* sqrt(2), points[i].getScale()*sqrt(2));
    }

    p.end(); // Don't forget this line!


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
      // p.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::SquareCap));

        for(int i = 0;i < matches.size(); i++){
//cout<<matches[i].distance<<endl;
            if(points1[matches[i].firstMatch].getScale() < 5 * points2[matches[i].secondMatch].getScale() &&
                points1[matches[i].firstMatch].getScale() > 0.2 * points2[matches[i].secondMatch].getScale()){
                p.setPen(QColor(abs(rand()) % 256, abs(rand()) % 256, abs(rand()) % 256));
                p.drawLine(points1[matches[i].firstMatch].getY(), points1[matches[i].firstMatch].getX(), points2[matches[i].secondMatch].getY(), points2[matches[i].secondMatch].getX());

                auto& p1 = points1[matches[i].firstMatch];
                p.drawEllipse(QPointF(p1.getY(),p1.getX()), p1.getScale()* sqrt(2), p1.getScale()*sqrt(2));
                auto& p2 = points2[matches[i].secondMatch];
                p.drawEllipse(QPointF(p2.getY(),p2.getX()), p2.getScale()* sqrt(2), p2.getScale()*sqrt(2));
            }

        }

         p.end(); // Don't forget this line!

         final.save("EMERGENCY.png");

     return final;
}


QImage drawMatches(const CVImage &first, const CVImage &second, vector<FeaturePoint> points1, vector<FeaturePoint> points2, vector<Dmatch> matches){
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
      // p.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::SquareCap));

        for(int i = 0;i < matches.size(); i++){
//cout<<matches[i].distance<<endl;
            if(points1[matches[i].firstMatch].getScale() < 5 * points2[matches[i].secondMatch].getScale() &&
                points1[matches[i].firstMatch].getScale() > 0.2 * points2[matches[i].secondMatch].getScale()){
                p.setPen(QColor(abs(rand()) % 256, abs(rand()) % 256, abs(rand()) % 256));
                p.drawLine(points1[matches[i].firstMatch].getY(), points1[matches[i].firstMatch].getX(), points2[matches[i].secondMatch].getY(), points2[matches[i].secondMatch].getX());

                auto& p1 = points1[matches[i].firstMatch];
                p.drawEllipse(QPointF(p1.getY(),p1.getX()), p1.getScale()* sqrt(2), p1.getScale()*sqrt(2));
                auto& p2 = points2[matches[i].secondMatch];
                p.drawEllipse(QPointF(p2.getY(),p2.getX()), p2.getScale()* sqrt(2), p2.getScale()*sqrt(2));
            }

        }

         p.end(); // Don't forget this line!

         final.save("EMERGENCY.png");

     return final;
}


QImage makePanorama(CVImage &first, CVImage &secondimage,double  homographyMatrix[9]){

        QTransform transform(homographyMatrix[4], homographyMatrix[1], homographyMatrix[7], homographyMatrix[3],
                homographyMatrix[0], homographyMatrix[6], homographyMatrix[5], homographyMatrix[2],
                homographyMatrix[8]);

//            QTransform transform(homographyMatrix[0], homographyMatrix[1], homographyMatrix[2], homographyMatrix[3],
//                    homographyMatrix[4], homographyMatrix[5], homographyMatrix[6], homographyMatrix[7],
//                    homographyMatrix[8]);

        cout<<"LESGO";
        for(int i =0;i < 9;i++){
            cout<<homographyMatrix[i]<<endl;
        }


        int width = 3000, height = 15000;
        QImage panorama(width, height, QImage::Format_RGB32);
        QPainter painter(&panorama);

        QImage image1(first.toQImage());
        QImage image2(secondimage.toQImage());

        image1.save("_TEST1.png");
        image2.save("_TEST2.png");

        int dx = 0, dy = 0;


        painter.drawImage(dx, dy, image1);

        painter.setTransform(transform);
        painter.drawImage(dx, dy, image2);

        painter.end();


        return panorama;

}

