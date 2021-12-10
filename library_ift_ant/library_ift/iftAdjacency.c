#include "iftAdjacency.h"

/*! \file
 * Author: Alexandre Falcao
 * Date: Aug 22, 2011
 * Last Update: Aug 22, 2011
 */

/* 3D adjacency relations */

iftAdjRel  *iftCreateAdjRel(int n) /*! \brief Allocates memory for a
				      3D adjacency relation */
{
  iftAdjRel *A=(iftAdjRel *)calloc(1,sizeof(iftAdjRel ));
  
  A->dx = (int *)iftAllocIntArray(n);
  A->dy = (int *)iftAllocIntArray(n);
  A->dz = (int *)iftAllocIntArray(n);
  A->n  = n;

  return(A);
}

void     iftDestroyAdjRel(iftAdjRel **A) /*! \brief Deallocates memory for a
					     3D adjacency relation */
{
  iftAdjRel *aux=*A;
  
  aux = *A;
  if (aux != NULL){
    if (aux->dx != NULL) free(aux->dx);
    if (aux->dy != NULL) free(aux->dy);
    if (aux->dz != NULL) free(aux->dz);
    free(aux);
    *A = NULL;
  }   
}

iftAdjRel  *iftSpheric(float r) /*! \brief Creates a 3D ball of radius r as
				 adjacency relation */
{
  iftAdjRel *A=NULL;
  int i,j,k,n,r0,r2,d,dx,dy,dz,i0=0;
  float *dr,aux;

  n=0;
  r0 = (int)r;
  r2  = (int)(r*r + 0.5);
  for(dz=-r0;dz<=r0;dz++)
    for(dy=-r0;dy<=r0;dy++)
      for(dx=-r0;dx<=r0;dx++)
      if(((dx*dx)+(dy*dy)+(dz*dz)) <= r2)
	n++;
	
  A = iftCreateAdjRel(n);
  i=0;
  for(dz=-r0;dz<=r0;dz++)
    for(dy=-r0;dy<=r0;dy++)
      for(dx=-r0;dx<=r0;dx++)
	if(((dx*dx)+(dy*dy)+(dz*dz)) <= r2){
	  A->dx[i]=dx;
	  A->dy[i]=dy;
	  A->dz[i]=dz;
	  if ((dx==0)&&(dy==0)&&(dz==0))
	    i0 = i;
	  i++;	  
	}

  /* shift to right and place central voxel at first */

  for (i=i0; i > 0; i--) {
    dx = A->dx[i];
    dy = A->dy[i];
    dz = A->dz[i];
    A->dx[i] = A->dx[i-1];
    A->dy[i] = A->dy[i-1];
    A->dz[i] = A->dz[i-1];
    A->dx[i-1] = dx;
    A->dy[i-1] = dy;
    A->dz[i-1] = dz;
  }

  /* sort by radius, so the 6 closest neighbors will come first */

  dr = iftAllocFloatArray(A->n);  
  for (i=0; i < A->n; i++) {
    dr[i] = A->dx[i]*A->dx[i] + A->dy[i]*A->dy[i] + A->dz[i]*A->dz[i];  
  }

  for (i=1; i < A->n-1; i++){
    k = i;
    for (j=i+1; j < A->n; j++)
      if (dr[j] < dr[k]){
	k = j;
      }
    aux   = dr[i];
    dr[i] = dr[k];
    dr[k] = aux;
    d        = A->dx[i];
    A->dx[i] = A->dx[k];
    A->dx[k] = d;
    d        = A->dy[i];
    A->dy[i] = A->dy[k];
    A->dy[k] = d;
    d        = A->dz[i];
    A->dz[i] = A->dz[k];
    A->dz[k] = d;
  }

  free(dr);


  return(A);
}

iftAdjRel *iftCircular(float r)
{
  iftAdjRel *A=NULL;
  int i,j,k,n,dx,dy,r0,r2,d,i0=0;
  float *dr,aux;

  n=0;

  r0  = (int)r;
  r2  = (int)(r*r + 0.5);
  for(dy=-r0;dy<=r0;dy++)
    for(dx=-r0;dx<=r0;dx++)
      if(((dx*dx)+(dy*dy)) <= r2)
	n++;
	
  A = iftCreateAdjRel(n);
  i=0;
  for(dy=-r0;dy<=r0;dy++)
    for(dx=-r0;dx<=r0;dx++)
      if(((dx*dx)+(dy*dy)) <= r2){
	A->dx[i]=dx;
	A->dy[i]=dy;
	A->dz[i]=0;
	if ((dx==0)&&(dy==0))
	  i0 = i;
	i++;
      }

  /* shift to right and place central pixel at first */

  for (i=i0; i > 0; i--) {
    dx = A->dx[i];
    dy = A->dy[i];
    A->dx[i] = A->dx[i-1];
    A->dy[i] = A->dy[i-1];
    A->dx[i-1] = dx;
    A->dy[i-1] = dy;
  }

  /* sort by radius, so the 4 closest neighbors will come first */

  dr = iftAllocFloatArray(A->n);  
  for (i=0; i < A->n; i++) {
    dr[i] = A->dx[i]*A->dx[i] + A->dy[i]*A->dy[i];  
  }

  for (i=1; i < A->n-1; i++){
    k = i;
    for (j=i+1; j < A->n; j++)
      if (dr[j] < dr[k]){
	k = j;
      }
    aux   = dr[i];
    dr[i] = dr[k];
    dr[k] = aux;
    d        = A->dx[i];
    A->dx[i] = A->dx[k];
    A->dx[k] = d;
    d        = A->dy[i];
    A->dy[i] = A->dy[k];
    A->dy[k] = d;
  }

  free(dr);

  return(A);
}


iftAdjRel *iftClockCircular(float r)
{
  iftAdjRel *A=NULL;
  int i,j,k,n,dx,dy,r0,r2,d,i0=0;
  float *da,*dr,aux;

  n=0;

  r0  = (int)r;
  r2  = (int)(r*r + 0.5);
  for(dy=-r0;dy<=r0;dy++)
    for(dx=-r0;dx<=r0;dx++)
      if(((dx*dx)+(dy*dy)) <= r2)
	n++;
	
  A = iftCreateAdjRel(n);
  i=0;
  for(dy=-r0;dy<=r0;dy++)
    for(dx=-r0;dx<=r0;dx++)
      if(((dx*dx)+(dy*dy)) <= r2){
	A->dx[i]=dx;
	A->dy[i]=dy;
	A->dz[i]=0;
	if ((dx==0)&&(dy==0))
	  i0 = i;
	i++;
      }

  /* Set clockwise */
  
  da = iftAllocFloatArray(A->n);
  dr = iftAllocFloatArray(A->n);
  for (i=0; i < A->n; i++) {
    dx = A->dx[i];
    dy = A->dy[i];
    dr[i] = (float)sqrtf((dx*dx) + (dy*dy));
    if (i != i0){ 
      da[i] = atan2(-dy,-dx)*180.0/PI;
      if (da[i] < 0.0)
	da[i] += 360.0;
    }
  }
  da[i0] = 0.0;
  dr[i0] = 0.0;

  /* place central pixel at first */
  
  aux    = da[i0];
  da[i0] = da[0];
  da[0]  = aux;
  aux    = dr[i0];
  dr[i0] = dr[0];
  dr[0]  = aux;
  d         = A->dx[i0];
  A->dx[i0] = A->dx[0];
  A->dx[0]  = d;
  d         = A->dy[i0];
  A->dy[i0] = A->dy[0];
  A->dy[0]  = d;

  /* sort by angle */
  
  for (i=1; i < A->n-1; i++){
    k = i;
    for (j=i+1; j < A->n; j++)
      if (da[j] < da[k]){
	k = j;
      }
    aux   = da[i];
    da[i] = da[k];
    da[k] = aux;
    aux   = dr[i];
    dr[i] = dr[k];
    dr[k] = aux;
    d   = A->dx[i];
    A->dx[i] = A->dx[k];
    A->dx[k] = d;
    d        = A->dy[i];
    A->dy[i] = A->dy[k];
    A->dy[k] = d;
  }

  /* sort by radius for each angle */
  
  for (i=1; i < A->n-1; i++){
    k = i;
    for (j=i+1; j < A->n; j++)
      if ((dr[j] < dr[k])&&(da[j]==da[k])){
	k = j;
      }
    aux   = dr[i];
    dr[i] = dr[k];
    dr[k] = aux;
    d        = A->dx[i];
    A->dx[i] = A->dx[k];
    A->dx[k] = d;
    d        = A->dy[i];
    A->dy[i] = A->dy[k];
    A->dy[k] = d;
  }

  free(dr);
  free(da);

  return(A);
}

iftAdjRel *iftRightSide(iftAdjRel *A, float r){
  iftAdjRel *R=NULL;
  int i;
  float d;

  for (i=0; i < A->n; i++) 
    if (A->dz[i]!=0)
      iftError("It must be a 2D adjacency relation","iftRightSide2D");

  /* Let p -> q be an arc represented by the increments dx,dy,dz. Its
     right side at distance r is given by the increments Dx = (-dy/d +
     dx/2)*r and Dy = dx/d + dy/2, where d=sqrt(dx²+dy²). */

  R = iftCreateAdjRel(A->n);
  for (i=0; i < R->n; i++){
    d  = sqrt(A->dx[i]*A->dx[i] + A->dy[i]*A->dy[i]);
    if (d != 0){
      R->dx[i] = ROUND( ((float)A->dx[i]/2.0)-((float)A->dy[i]/d)*r );
      R->dy[i] = ROUND( ((float)A->dx[i]/d)+((float)A->dy[i]/2.0)*r );
      R->dz[i] = 0.0;
    }
  }

  return(R);
}

iftAdjRel *iftLeftSide(iftAdjRel *A, float r)
{
  iftAdjRel *L=NULL;
  int i;
  float d;

  for (i=0; i < A->n; i++) 
    if (A->dz[i]!=0)
      iftError("It must be a 2D adjacency relation","iftLeftSide2D");

  /* Let p -> q be an arc represented by the increments dx,dy. Its
     left side is given by the increments Dx = dy/d + dx/2 and Dy =
     -dx/d + dy/2, where d=sqrt(dx²+dy²). */

  L = iftCreateAdjRel(A->n);
  for (i=0; i < L->n; i++){
    d  = sqrt(A->dx[i]*A->dx[i] + A->dy[i]*A->dy[i]);
    if (d != 0){
      L->dx[i] = ROUND( ((float)A->dx[i]/2.0)+((float)A->dy[i]/d)*r );
      L->dy[i] = ROUND( ((float)A->dy[i]/2)-((float)A->dx[i]/d)*r );
      L->dz[i] = 0;
    }
  }
  
  return(L);
}
