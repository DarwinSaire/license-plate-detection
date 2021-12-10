#include "iftPlane.h"

iftPlane *iftCreatePlane()
{
  iftPlane *pl=(iftPlane *)calloc(1,sizeof(iftPlane));

  iftSetPlanePos(pl,0,0,0);
  iftSetPlaneOrient(pl,0.0,0.0,1.0);

  return(pl);
}

void iftDestroyPlane(iftPlane **pl)
{
  iftPlane *aux=*pl;

  if (aux != NULL){
    free(aux);
    *pl = NULL;
  }
}

void iftTranslatePlane(iftPlane *pl, float Tx, float Ty, float Tz){
  pl->pos.x += Tx;
  pl->pos.y += Ty;
  pl->pos.z += Tz;
}

void iftRotatePlane(iftPlane *pl, char axis, float theta)
{
  iftMatrix *R=iftRotationMatrix(axis,theta);
  iftVector N;
  
  N = iftTransformVector(R,pl->normal);

  iftSetPlaneOrient(pl,N.x,N.y,N.z);
}

void iftSetPlanePos(iftPlane *pl, float x, float y, float z)
{
  pl->pos.x = x;
  pl->pos.y = y;
  pl->pos.z = z;
}

void iftSetPlaneOrient(iftPlane *pl, float Nx, float Ny, float Nz)
{
  float m = sqrtf(Nx*Nx + Ny*Ny + Nz*Nz); 

  if (m==0.0)
    iftError("There is no orientation","iftSetPlaneOrient");

  pl->normal.x = Nx/m;
  pl->normal.y = Ny/m;
  pl->normal.z = Nz/m;
}

