#include "iftColor.h"

iftColorTable *iftCreateColorTable(int ncolors)
{
  iftColorTable *ctb = (iftColorTable *) calloc(1,sizeof(iftColorTable));
  int i;

  ctb->color = (iftColor *)calloc(ncolors,sizeof(iftColor));
  if (ctb->color == NULL) 
    iftError(MSG1,"iftCreateColorTable");

  ctb->ncolors = ncolors;

  iftRandomSeed();
  for (i=0; i < ncolors; i++) {
    ctb->color[i].val[0] = iftRandomInteger(0,255);
    ctb->color[i].val[1] = iftRandomInteger(0,255);
    ctb->color[i].val[2] = iftRandomInteger(0,255);
  }

  return(ctb);
}

void iftDestroyColorTable(iftColorTable **ctb)
{
  iftColorTable *aux=*ctb;

  if (aux != NULL) {
    free(aux->color);
    free(aux);
    ctb = NULL;
  }

}

/* 
 http://www.equasys.de/colorconversion.html#YCbCr-RGBColorFormatConversion
 for 24-bit color images
*/


iftColor iftRGBtoYCbCr(iftColor cin)
{
  iftColor cout;
  int i;


  cout.val[0]=(int)(0.299*(float)cin.val[0]+
		    0.587*(float)cin.val[1]+
		    0.114*(float)cin.val[2]);
  cout.val[1]=(int)(-0.169*(float)cin.val[0]+
		    -0.331*(float)cin.val[1]+
		    0.500*(float)cin.val[2])+128;
  cout.val[2]=(int)(0.500*(float)cin.val[0]+
		    -0.419*(float)cin.val[1]+
		    -0.081*(float)cin.val[2])+128;
   
  for (i=0; i < 3; i++)  { 
    if (cout.val[i]>255) cout.val[i]=255;
    if (cout.val[i]<0)   cout.val[i]=0;
  }
  
  return(cout);
}

// http://www.equasys.de/colorconversion.html#YCbCr-RGBColorFormatConversion
// http://www.bluebit.gr/matrix-calculator/calculate.aspx

iftColor iftYCbCrtoRGB(iftColor cin)
{
  iftColor cout;
  int i;

  
      cout.val[0]=(int)(0.971*(float)cin.val[0]+
		       -0.053*((float)cin.val[1]-128)+
			1.402*((float)cin.val[2]-128));
      
      cout.val[1]=(int)(0.971*(float)cin.val[0]+
			-0.395*((float)cin.val[1]-128)+
			-0.714*((float)cin.val[2]-128));

      cout.val[2]=(int)(0.971*(float)cin.val[0]+
			1.721*((float)cin.val[1]-128)+
			0.001*((float)cin.val[2]-128));
  
      for (i=0; i < 3; i++)  { 
	if (cout.val[i]>255) cout.val[i]=255;
	if (cout.val[i]<0)   cout.val[i]=0;
      }
			
      return(cout);
}

