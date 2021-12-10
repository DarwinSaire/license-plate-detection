#ifndef IFT_IMAGE_H_
#define IFT_IMAGE_H_

#include "iftCommon.h"
#include "iftColor.h"
#include "iftFIFO.h"
#include "iftAdjacency.h"
#include "iftPlane.h"
#include "iftMatrix.h"

/*typedef struct ift_image_dir{
  int    *val;
  float    *dirx, *diry, *dirz;
  ushort *Cb,*Cr;  
  int xsize,ysize,zsize;
  float dx,dy,dz;
  int *tby, *tbz;
  int maxval, minval, n;
}iftImageDir;*/

typedef struct ift_image {
  int    *val;
  float *dirx, *diry, *dirz;
  ushort *Cb,*Cr;  
  int xsize,ysize,zsize;
  float dx,dy,dz;
  int *tby, *tbz;
  int maxval, minval, n;
} iftImage;

#define iftGetXCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) % (s)->xsize)
#define iftGetYCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) / (s)->xsize)
#define iftGetZCoord(s,p) ((p) / (((s)->xsize)*((s)->ysize)))
#define iftGetVoxelIndex(s,v) ((v.x)+(s)->tby[(v.y)]+(s)->tbz[(v.z)])
#define iftDiagonalSize(s) (sqrtf(s->xsize*s->xsize + s->ysize*s->ysize + s->zsize*s->zsize))

iftVoxel  iftGetVoxelCoord(iftImage *img, int p);
iftVoxel  iftGetAdjacentVoxel(iftAdjRel *A, iftVoxel u, int adj);
iftImage *iftCreateImage(int xsize,int ysize,int zsize);
void      iftDestroyImage(iftImage **img);
void      iftUpdateMinMax(iftImage *img);
void      iftCopyVoxelSize(iftImage *img1, iftImage *img2);
char      iftValidVoxel(iftImage *img, iftVoxel v);
int       iftMaximumValue(iftImage *img);
int       iftMinimumValue(iftImage *img);
iftImage *iftReadImage(char *filename);
void      iftWriteImage(iftImage *img, char *filename);
extern iftImage *iftReadImageP5(char *filename);
extern void      iftWriteImageP5(iftImage *img, char *filename);
iftImage *iftReadImageP6(char *filename);
void      iftWriteImageP6(iftImage *img, char *filename);
iftImage *iftReadImageP2(char *filename);
void      iftWriteImageP2(iftImage *img, char *filename);
iftImage *iftRemoveROI(iftImage *img, iftImage *bin, iftVoxel *pos);
iftImage *iftInsertROI(iftImage *img, iftImage *bin, iftImage *roi, iftVoxel  pos);
iftImage  *iftCopyImage(iftImage *img);
iftImage  *iftCreateCuboid(int xsize, int ysize, int zsize);
iftImage  *iftCSVtoImage(char *filename);
char       iftAdjacentVoxels(iftImage *img, iftAdjRel *A, iftVoxel u, iftVoxel v);
iftImage  *iftImageGradientMagnitude(iftImage *img, iftAdjRel *A);
iftImage *iftAddFrame(iftImage *img, int sz, int value);
iftImage *iftRemFrame(iftImage *fimg, int sz);
void      iftSetImage(iftImage *img, int value);
iftImage *iftGetSlice(iftImage *img, iftPlane *pl, int xsize, int ysize);
iftVoxel  iftGeometricCenterVoxel(iftImage *bin);
iftPoint  iftGeometricCenter(iftImage *bin);
iftImage *iftCropObjects(iftImage *bin);
iftImage *iftResliceImage(iftImage *img, iftPlane *pl, int xsize, int ysize, int zsize);
iftImage *iftCropImage(iftImage *img, iftVoxel uo, iftVoxel uf);
void      iftGetDisplayRange(iftImage *img, int *lower, int *higher);
iftImage *iftImageCb(iftImage *img);
iftImage *iftImageCr(iftImage *img);
iftImage *iftImageRed(iftImage *img);
iftImage *iftImageGreen(iftImage *img);
iftImage *iftImageBlue(iftImage *img);

iftImage  *iftImageGradient(iftImage *img, iftAdjRel *A);
#endif


