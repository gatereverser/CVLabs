
#include "cvkernel.h"

CVKernel::CVKernel(int rows, int columns, int kernelX, int kernelY)
{
    this->rowCount = rows;
    this->columnCount = columns;
    data = make_unique<double[]>(rows * columns);

    if(kernelX!=-1){
        this->centerX = kernelX;
    }
    else{
         this->centerX = columnCount / 2;
    }

    if(kernelY!=-1){
        this->centerY = kernelY;
    }
    else{
         this->centerY = rowCount / 2;
    }
}

CVKernel::CVKernel(int rows, int columns, double *data, int kernelX, int kernelY)
{
    this->rowCount = rows;
    this->columnCount = columns;

    if(kernelX!=-1){
        this->centerX = kernelX;
    }
    else{
         this->centerX = columnCount / 2;
    }

    if(kernelY!=-1){
        this->centerY = kernelY;
    }
    else{

         this->centerY = rowCount / 2;
    }

    this->data = make_unique<double[]>(rows * columns);
    for (int i = 0; i < rows * columns; i++)
        this->data[i] = data[i];

}
CVKernel::CVKernel(const  CVKernel &anotherKernel)
{
    rowCount = anotherKernel.rowCount;
    columnCount= anotherKernel.columnCount;
    centerX = anotherKernel.centerX;
    centerY = anotherKernel.centerY;

    data = std::make_unique<double[]>(rowCount*columnCount);
    for (int i = 0; i < rowCount*columnCount; ++i)
        data[i] = anotherKernel.data[i];
}

CVKernel::~CVKernel()
{

}

int CVKernel::getRowCount()
{
    return rowCount;
}

int CVKernel::getColumnCount()
{
    return columnCount;
}


int CVKernel::getCenterX()
{
    return centerX;
}

int CVKernel::getCenterY()
{
    return centerY;
}

double *CVKernel::getData()
{
    return data.get();
}

double CVKernel::getValue(int row, int column)
{
    return data[ row * columnCount + column];
}

void CVKernel::setValue(int row, int column, double value)
{
    data[ row * columnCount + column] = value;
}

