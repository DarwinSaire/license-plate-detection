#ifndef IFT_COLOR_H_
#define IFT_COLOR_H_

#include "iftCommon.h"

typedef struct ift_color {
  int val[3];
} iftColor;

typedef struct ift_colortable {
  iftColor *color;
  int ncolors;
} iftColorTable;

iftColorTable *iftCreateColorTable(int ncolors);
void iftDestroyColorTable(iftColorTable **ctb);
iftColor iftRGBtoYCbCr(iftColor cin);
iftColor iftYCbCrtoRGB(iftColor cin);

#endif
