#ifndef IFT_ADJACENCY_H_
#define IFT_ADJACENCY_H_

#include "iftCommon.h"

/*  Adjacency relation in 3D */ 
 
typedef struct ift_adjrel { 
  int *dx,*dy,*dz; /* displacements to achieve the n adjacent voxels. */ 
  int  n; /* number of adjacent voxels. */ 
} iftAdjRel;

iftAdjRel  *iftCreateAdjRel(int n); /* Allocates memory for a 3D
				      adjacency relation */

void     iftDestroyAdjRel(iftAdjRel **A); /* Deallocates memory for a
					     3D adjacency relation */

iftAdjRel  *iftSpheric(float r); /* Creates a 3D ball of radius r as
				 adjacency relation */

iftAdjRel  *iftCircular(float r); /* Creates a 2D ball of radius r on
				 the xy plane as adjacency relation */

iftAdjRel  *iftClockCircular(float r); /* Creates a 2D ball of radius
				 r on the xy plane as adjacency
				 relation for contour pixel
				 labeling */

iftAdjRel *iftRightSide(iftAdjRel *A, float r);

iftAdjRel *iftLeftSide(iftAdjRel *A, float r);

#endif
