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


void Convolute(CVImage source, CVImage &dest, CVKernel kernel, BorderWrappingType type = BorderWrappingType::ZeroBorder);

void CVSobelX(CVImage source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelSeparateX(CVImage source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelY(CVImage source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobelSeparateY(CVImage source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder, bool isNorm = false);
void CVSobel(CVImage source, CVImage &dest, BorderWrappingType type = BorderWrappingType::ZeroBorder);


#endif // CV

