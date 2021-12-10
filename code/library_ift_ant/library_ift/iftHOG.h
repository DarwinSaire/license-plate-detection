
//#include "iftCommon.h"
#include "iftHistogram.h"

typedef struct ift_hog { 
  iftHist *hist;
  int cell; 
  int block;
  //iftImage img;
  int nwidth, nheight;
  int bin;
} iftHog;

iftHog  *iftCreateHog(iftImage *img, int icell, int iblock, int nbin);

//void      iftDestroyHog(iftHist **H);

void splitImage( iftImage *img, iftHog* Hog );

iftHist *iftRunHog(iftImage *img, int icell, int iblock, int nbin);

