#ifndef _IFTMATRIX_H_
#define _IFTMATRIX_H_

#include "iftCommon.h"

typedef struct ift_matrix {
  float **val;
  int ncols,nrows;
} iftMatrix;

iftMatrix *iftCreateMatrix(int ncols,int nrows);
void       iftDestroyMatrix(iftMatrix **A);
iftMatrix *iftCopyMatrix(iftMatrix *A);
iftMatrix *iftTransposeMatrix(iftMatrix *A);
iftMatrix *iftMultMatrices(iftMatrix *A, iftMatrix *B);
void       iftPrintMatrix(iftMatrix *A);
iftMatrix *iftRotationMatrix(char axis, float theta); 
iftMatrix *iftTranslationMatrix(iftVector T);
iftMatrix *iftScaleMatrix(iftScale S);
iftVector  iftTransformVector(iftMatrix *A, iftVector u);
iftPoint   iftTransformPoint(iftMatrix *A, iftPoint  u);
iftVoxel   iftTransformVoxel(iftMatrix  *A, iftVoxel  u);
iftMatrix *iftIdentityMatrix(int ncols);
iftMatrix *iftRotationMatrixToAlignVectorWithZ(iftVector v);
iftMatrix *iftRotationMatrixToAlignZWithVector(iftVector v);



#endif
