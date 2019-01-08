#include "superstructure.h"

SuperStructure::SuperStructure(float x, float y, float w, float h, float d, int type)
    :indexBuf(QOpenGLBuffer::IndexBuffer),
      X(x),
      Y(y),
      Width(w),
      Height(h),
      Depth(d),
      typeStruct(type)
{

}
