#pragma once
#include <iostream>
#include <memory>


using namespace std;


class CVKernel
{

private:
    int centerX;
    int centerY;
    int rowCount;
    int columnCount;
    unique_ptr<double[]> data;

public:
    CVKernel(int rows, int columns, int kernelX = -1, int kernelY = -1);
    CVKernel(int rows, int columns, double *data, int kernelX = -1, int kernelY = -1);
    CVKernel(const  CVKernel &anotherKernel);
    ~CVKernel();

    int getRowCount();
    int getColumnCount();
    int getCenterX() ;
    int getCenterY() ;
    double *getData() ;
    double getValue(int row, int column);
    void setValue(int row, int column, double value);
};



