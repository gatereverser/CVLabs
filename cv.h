#ifndef CV
#define CV

#include "cvimage.h"
#include "cvkernel.h"
#include"amp.h"



enum class BorderWrappingType{
    ZeroBorder,
    CopyBorder,
    ReflectBorder,
    WrapupBorder
};


void Convolute(const CVImage &source, CVImage &dest, CVKernel kernel, BorderWrappingType type = BorderWrappingType::ZeroBorder);

void CVSobelX(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelSeparateX(const CVImage source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelY(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelSeparateY(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobel(const CVImage &source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder);


#endif // CV

