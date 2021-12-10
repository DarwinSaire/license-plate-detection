
#include "iftCommon.h"
#include "iftImage.h"

typedef struct ift_hist { 
  int *values;
  int bin; 
} iftHist;

iftHist  *iftCreateHistogram(int n);

void      iftDestroyHistogram(iftHist **H);

iftHist  *iftRunHistogram(iftImage *img, int x1, int y1, int x2, int y2 );

iftHist  *iftUnionHistogram( iftHist *conj_hist, int *ids, int n );