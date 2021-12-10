#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <float.h>
//#include <time.h>
//#include<sys\timeb.h>
//#include <sys/time.h>

#include "iftImage.h"
#include "iftHOG.h"

/*iftImage *Dilate(iftImage *img, iftAdjRel *A)
{
  iftImage *dil=iftCreateImage(img->xsize,img->ysize,img->zsize);
  for(int p=0;p<img->n;p++){
    iftVoxel u = iftGetVoxelCoord(img,p);
    dil->val[p] = img->val[p];
    for (int i=1; i < A->n; i++) {
      iftVoxel v = iftGetAdjacentVoxel(A,u,i);
      if (iftValidVoxel(img,v)){
	  int q = iftGetVoxelIndex(img,v);
	  if (img->val[q]>dil->val[p])
	    dil->val[p] = img->val[q];
	}
    }
  }

  return(dil);
}

iftImage *Erode(iftImage *img, iftAdjRel *A)
{
  iftImage *ero=iftCreateImage(img->xsize,img->ysize,img->zsize);

  for (int p=0; p < img->n; p++) {
    iftVoxel u = iftGetVoxelCoord(img,p);
    ero->val[p] = img->val[p];
    for (int i=1; i < A->n; i++) {
      iftVoxel v = iftGetAdjacentVoxel(A,u,i);
      if (iftValidVoxel(img,v)){
	  int q = iftGetVoxelIndex(img,v);
	  if (img->val[q]<ero->val[p])
	    ero->val[p] = img->val[q];
      }
    }
  }

  return(ero);
}


iftImage *Close(iftImage *img, iftAdjRel *A)
{
  iftImage *dil,*ero;

  dil = Dilate(img,A);
  ero = Erode(dil,A);
  iftDestroyImage(&dil);
  return(ero);

}*/

/*iftImage *Sub(iftImage *img1, iftImage *img2)
{
  iftImage *img3=NULL;
  int p;

  iftVerifyImageDomains(img1,img2,"Sub");

  img3 = iftCreateImage(img1->xsize,img1->ysize,img1->zsize);

  for (p=0; p < img2->n; p++)
    if (img1->val[p]>=img2->val[p])
      img3->val[p]=img1->val[p]-img2->val[p];

  return(img3);
}*/

//int main(int argc, char *argv[]) 
int main(void)
{
  iftImage  *img[3];
  iftAdjRel *A;
  char       ext[10],*pos;
  //timer     *t1=NULL,*t2=NULL;

  /*--------------------------------------------------------*/

  /*void *trash = malloc(1);                 
  struct mallinfo info;   
  int MemDinInicial, MemDinFinal;
  free(trash); 
  info = mallinfo();
  MemDinInicial = info.uordblks;*/

  /*--------------------------------------------------------*/

  char *argv[4] = {"program","wedge.pgm", "1", "main" };

  /*if (argc!=3){
    iftError("Usage: iftBottomHat <image.pgm> <adj. radius>","main");
  }*/

  pos = strrchr(argv[1],'.') + 1;
  sscanf(pos,"%s",ext);
  
  if (strcmp(ext,"pgm")==0){
    img[0]   = iftReadImageP2(argv[1]);    
  }else{
    printf("Invalid image format: %s\n",ext);
    exit(-1);
  }

  A = iftCircular(atof(argv[2]));

  img[1]=img[2]=NULL;

  img[1] = iftImageGradientMagnitude(img[0],A);

  //t1     = iftTic();

  //img[1] = Close(img[0],A);    
  //img[2] = Sub(img[1],img[0]);    

  //t2     = iftToc();
  //fprintf(stdout,"Bottom-Hat in %f ms\n",iftCompTime(t1,t2));

  iftWriteImageP2(img[1],"result.pgm");    

  iftDestroyAdjRel(&A);
  iftDestroyImage(&img[0]);
  if (img[1]!=NULL) 
    iftDestroyImage(&img[1]);

  /* ---------------------------------------------------------- */

  /*info = mallinfo();
  MemDinFinal = info.uordblks;
  if (MemDinInicial!=MemDinFinal)
    printf("\n\nDinamic memory was not completely deallocated (%d, %d)\n",
	   MemDinInicial,MemDinFinal);   */

  system("pause");
  return(0);

}
