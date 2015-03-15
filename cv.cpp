#include"cv.h"

void Convolute(const CVImage &source, CVImage &dest, CVKernel kernel, BorderWrappingType type)
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
                    double adding =kernel.getValue(u,v);
                    int testI = i-(u-kernel.getCenterY());
                    int testJ = j-(v-kernel.getCenterX());

                    if(testI < dest.getHeight() && testJ < dest.getWidth() && testI >= 0 && testJ >= 0){
                        adding *= source.getPixel(testI, testJ);

                    }
                    else{
                        switch(type)
                        {
                            case BorderWrappingType::ZeroBorder:
                                adding =  0;
                                break;
                            case BorderWrappingType::CopyBorder:
                                adding *= source.getPixel(min(max(testI,0), source.getHeight()-1), min(max(testJ,0), source.getWidth()-1));
                                break;
                            case BorderWrappingType::ReflectBorder:
                                if(testI < 0) testI = testI * (-1);
                                if(testJ < 0) testJ = testJ * (-1);
                                if(testI >= source.getHeight()) testI = 2 * (source.getHeight() - 1) - testI;
                                if(testJ >= source.getWidth()) testJ = 2 * (source.getWidth() - 1) - testJ;
                                adding *= source.getPixel(testI, testJ);
                                break;
                            case BorderWrappingType::WrapupBorder:

                                if (testI < 0)
                                    testI = source.getHeight() + testI;
                                if (testI >= source.getHeight())
                                    testI = testI - source.getHeight();

                                if (testJ < 0)
                                    testJ  = source.getWidth() + testJ ;
                                if (testJ  >= source.getWidth())
                                    testJ  = testJ  - source.getWidth();

                                adding *=  source.getPixel(testI, testJ);
                        }
                    }
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
