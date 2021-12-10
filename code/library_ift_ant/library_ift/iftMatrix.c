#include "iftMatrix.h"

iftMatrix *iftCreateMatrix(int ncols,int nrows)
{
  iftMatrix *A=NULL;
  float *aux;
  int i;

  A = (iftMatrix *) calloc(1,sizeof(iftMatrix));
  if(A == NULL)
    iftError(MSG1,"iftCreateMatrix");

  aux    = (float *)  iftAllocFloatArray(nrows*ncols);
  A->val = (float **) calloc(nrows, sizeof(float *));
  if ((A->val == NULL)||(aux == NULL))
    iftError(MSG1,"iftCreateMatrix");

  A->val[0] = aux;
  for(i=1; i < nrows; i++) 
    A->val[i] = A->val[i-1] + ncols;

  A->ncols = ncols;
  A->nrows = nrows;
 
  return(A);
}

void iftDestroyMatrix(iftMatrix **A)
{
  iftMatrix *aux;

  aux = *A;
  if(aux != NULL){
    if(aux->val != NULL){
      if(*(aux->val) != NULL)
	free(*(aux->val));
      free(aux->val); 
    }
    free(aux);
    *A = NULL;
  }
}

iftMatrix *iftCopyMatrix(iftMatrix *A)
{
  iftMatrix *B;

  B = iftCreateMatrix(A->ncols, A->nrows);
  memcpy(B->val[0], A->val[0], A->ncols*A->nrows*sizeof(float));
  
  return(B);
}

void   iftPrintMatrix(iftMatrix *A)
{
  int x, y;
  
  fprintf(stdout,"\n");
  for(y=0; y < A->nrows; y++){
    for(x=0; x < A->ncols; x++)
      fprintf(stdout,"%3.5lf ", A->val[y][x]);
    fprintf(stdout,"\n");
  }
  fprintf(stdout,"\n");
}

iftMatrix *iftMultMatrices(iftMatrix *A, iftMatrix *B)
{
  iftMatrix *M = NULL;
  int i,j,k;
  
  if(A->ncols!=B->nrows)
    iftError("Cannot multiply matrices","iftMultMatrices");
  
  M = iftCreateMatrix(B->ncols, A->nrows);
  for(i=0; i < M->nrows; i++){
    for(j=0; j < M->ncols; j++){
       M->val[i][j] = 0.0;
       for (k=0; k<A->ncols; k++)
	 M->val[i][j] += A->val[i][k]*B->val[k][j];
    }
  }
  return(M);
}

iftMatrix *iftTransposeMatrix(iftMatrix *A)
{
  iftMatrix *B = NULL;
  int i,j;
    
  B = iftCreateMatrix(A->nrows, A->ncols);
  for(i=0; i < B->nrows; i++){
    for(j=0; j < B->ncols; j++){
      B->val[i][j] = A->val[j][i];
    }
  }
  return(B);
}

iftMatrix *iftRotationMatrix(char axis, float theta)
{
  iftMatrix *A;
  float cos_theta,sin_theta;

  A         = iftCreateMatrix(4,4);
  theta     = theta*PI/180.0;
  cos_theta = cosf(theta); 
  sin_theta = sinf(theta); 

  switch(axis) {

  case AXIS_X:

    A->val[0][0] = 1.0; A->val[0][1] = 0.0; A->val[0][2] = 0.0; A->val[0][3] = 0.0;
    A->val[1][0] = 0.0; A->val[1][1] = cos_theta; A->val[1][2] = -sin_theta; A->val[1][3] = 0.0;
    A->val[2][0] = 0.0; A->val[2][1] = sin_theta; A->val[2][2] = cos_theta;  A->val[2][3] = 0.0;
    A->val[3][0] = 0.0; A->val[3][1] = 0.0; A->val[3][2] = 0.0; A->val[3][3] = 1.0;  
    break;

  case AXIS_Y:

    A->val[0][0] = cos_theta;  A->val[0][1] = 0.0;  A->val[0][2] = sin_theta;  A->val[0][3] = 0.0;
    A->val[1][0] = 0.0;  A->val[1][1] = 1.0;  A->val[1][2] = 0.0;  A->val[1][3] = 0.0;
    A->val[2][0] = -sin_theta;  A->val[2][1] = 0.0;  A->val[2][2] = cos_theta;  A->val[2][3] = 0.0;
    A->val[3][0] = 0.0;  A->val[3][1] = 0.0;  A->val[3][2] = 0.0;  A->val[3][3] = 1.0;
    
    break;

  case AXIS_Z:

    A->val[0][0] = cos_theta;  A->val[0][1] = -sin_theta;  A->val[0][2] = 0.0;  A->val[0][3] = 0.0;
    A->val[1][0] = sin_theta;  A->val[1][1] =  cos_theta;  A->val[1][2] = 0.0;  A->val[1][3] = 0.0;
    A->val[2][0] = 0.0;  A->val[2][1] = 0.0;  A->val[2][2] = 1.0;  A->val[2][3] = 0.0;
    A->val[3][0] = 0.0;  A->val[3][1] = 0.0;  A->val[3][2] = 0.0;  A->val[3][3] = 1.0;
    
    break;

  default:
    iftError("Invalid option for the axis","iftRotationMatrix");
  }

  return(A);
}

iftMatrix *iftTranslationMatrix(iftVector T)
{
  iftMatrix *A;

  A = iftCreateMatrix(4,4);

  A->val[0][0] = 1.0;    A->val[0][1] = 0.0;    A->val[0][2] = 0.0;    A->val[0][3] = T.x;
  A->val[1][0] = 0.0;    A->val[1][1] = 1.0;    A->val[1][2] = 0.0;    A->val[1][3] = T.y;
  A->val[2][0] = 0.0;    A->val[2][1] = 0.0;    A->val[2][2] = 1.0;    A->val[2][3] = T.z;
  A->val[3][0] = 0.0;    A->val[3][1] = 0.0;    A->val[3][2] = 0.0;    A->val[3][3] = 1.0;

  return(A);
}

iftMatrix *iftScaleMatrix(iftScale S)
{
  iftMatrix *A;

  A = iftCreateMatrix(4,4);

  A->val[0][0] = S.x;    A->val[0][1] = 0.0;    A->val[0][2] = 0.0;    A->val[0][3] = 0;
  A->val[1][0] = 0.0;    A->val[1][1] = S.y;    A->val[1][2] = 0.0;    A->val[1][3] = 0;
  A->val[2][0] = 0.0;    A->val[2][1] = 0.0;    A->val[2][2] = S.z;    A->val[2][3] = 0;
  A->val[3][0] = 0.0;    A->val[3][1] = 0.0;    A->val[3][2] = 0.0;    A->val[3][3] = 1.0;

  return(A);
}

iftVector iftTransformVector(iftMatrix *A, iftVector u)
{
  iftMatrix *um,*res;
  iftVector v;

  um = iftCreateMatrix(1,4);
  um->val[0][0]=u.x;
  um->val[1][0]=u.y;
  um->val[2][0]=u.z;
  um->val[3][0]=1.0;
  res=iftMultMatrices(A,um);
  v.x = res->val[0][0];
  v.y = res->val[0][1];
  v.z = res->val[0][2];
  iftDestroyMatrix(&um);
  iftDestroyMatrix(&res);

  return(v);
}

iftPoint iftTransformPoint(iftMatrix *A, iftPoint u)
{
  iftMatrix *um,*res;
  iftPoint v;

  um = iftCreateMatrix(1,4);
  um->val[0][0]=u.x;
  um->val[1][0]=u.y;
  um->val[2][0]=u.z;
  um->val[3][0]=1.0;
  res=iftMultMatrices(A,um);
  v.x = res->val[0][0];
  v.y = res->val[0][1];
  v.z = res->val[0][2];
  iftDestroyMatrix(&um);
  iftDestroyMatrix(&res);

  return(v);
}

iftVoxel iftTransformVoxel(iftMatrix *A, iftVoxel u)
{
  iftMatrix *um,*res;
  iftVoxel v;

  um = iftCreateMatrix(1,4);
  um->val[0][0]=u.x;
  um->val[1][0]=u.y;
  um->val[2][0]=u.z;
  um->val[3][0]=1.0;
  res=iftMultMatrices(A,um);
  v.x = ROUND(res->val[0][0]);
  v.y = ROUND(res->val[0][1]);
  v.z = ROUND(res->val[0][2]);
  iftDestroyMatrix(&um);
  iftDestroyMatrix(&res);

  return(v);
}

iftMatrix *iftIdentityMatrix(int ncols)
{
  iftMatrix *I=iftCreateMatrix(ncols,ncols);
  int i;
  
  for (i=0; i < ncols; i++) 
    I->val[i][i] = 1.0;

  return(I);
}

/* This function aims at aligning a vector v with the direction of the
   axis Z. First, it rotates v by theta_x around the axis X until it
   falls on the xz plane. The value of theta_x can be obtained from
   the projection v_{yz} of v onto the yz plane as atan(vy/vz). It
   then rotates v (already on the XZ plane) of -theta_y until it
   aligns with the axis Z. The value of theta_y can be obtained from
   the projection v_{xz} of v onto the xz plane as atan(vx/vz). */

iftMatrix *iftRotationMatrixToAlignVectorWithZ(iftVector v)
{
  float theta_x,theta_y, m = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); 
  iftMatrix *Rx,*Ry,*M;

  if (m == 0.0)
    iftError("There is no vector","iftRotationMatrixToAlignVectorWithZ");

  /* Make sure it is normalized */

  v.x /= m;
  v.y /= m;
  v.z /= m;

  /* treat the special cases where the tangent is not computable */

  if (v.z == 0.0) {
    if (v.x == 0.0)
      theta_y = 0.0;
    else
      theta_y = 90.0;

    if (v.y == 0.0)
      theta_x = 0.0;
    else
      theta_x = 90.0;
  }else{
    theta_x = atanf(v.y/v.z)*180.0/PI;
    theta_y = atanf(v.x/v.z)*180.0/PI;
  }

  /* Compute the corresponding Rx and Ry matrices, and multiply them
     to create the desired matrix. */

  Rx = iftRotationMatrix(AXIS_X,theta_x);
  Ry = iftRotationMatrix(AXIS_Y,-theta_y);
  M  = iftMultMatrices(Ry,Rx);

  iftDestroyMatrix(&Ry);  
  iftDestroyMatrix(&Rx);

  return(M);
}

/* This function computes the inverse of the above matrix in order to
   align the axis Z with a vector v. Essentially, it has to compute
   the inverse of Ry(-theta_y)Rx(theta_x), which is
   Rx(-theta_x)Ry(theta_y). */

iftMatrix *iftRotationMatrixToAlignZWithVector(iftVector v)
{
  float theta_x,theta_y, m = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); 
  iftMatrix *Rx,*Ry,*M;

  if (m == 0.0)
    iftError("There is no vector","iftRotationMatrixToAlignVectorWithZ");

  v.x /= m;
  v.y /= m;
  v.z /= m;

  if (v.z == 0.0) {
    if (v.x == 0.0)
      theta_y = 0.0;
    else
      theta_y = 90.0;

    if (v.y == 0.0)
      theta_x = 0.0;
    else
      theta_x = 90.0;
  }else{
    theta_x = atanf(v.y/v.z)*180.0/PI;
    theta_y = atanf(v.x/v.z)*180.0/PI;
  }

  Rx = iftRotationMatrix(AXIS_X,-theta_x);
  Ry = iftRotationMatrix(AXIS_Y,theta_y);
  M  = iftMultMatrices(Rx,Ry);

  iftDestroyMatrix(&Ry);  
  iftDestroyMatrix(&Rx);

  return(M);
}
