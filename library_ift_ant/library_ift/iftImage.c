#include "iftImage.h"

/*---------------------- Public Functions ---------------------------*/

iftImage  *iftCreateImage(int xsize,int ysize,int zsize)
{
  iftImage *img=NULL;
  int     y,z,xysize;

  img = (iftImage *) calloc(1,sizeof(iftImage));
  if (img == NULL){
    iftError(MSG1,"iftCreateImage");
  }
  
  img->val     = iftAllocIntArray(xsize*ysize*zsize);
  img->dirx     = iftAllocFloatArray(xsize*ysize*zsize);
  img->diry     = iftAllocFloatArray(xsize*ysize*zsize);
  img->dirz     = iftAllocFloatArray(xsize*ysize*zsize);
  img->Cb=img->Cr=NULL;      
  img->xsize   = xsize;
  img->ysize   = ysize;
  img->zsize   = zsize;
  img->dx      = 1.0;
  img->dy      = 1.0;
  img->dz      = 1.0;
  img->tby     = iftAllocIntArray(ysize);
  img->tbz     = iftAllocIntArray(zsize);
  img->maxval  = 0;
  img->minval  = 0;
  img->n       = xsize*ysize*zsize;

  if (img->val==NULL || img->tbz==NULL || img->tby==NULL) {
    iftError(MSG1,"iftCreateImage");
  }

  img->tby[0]=0;
  for (y=1; y < ysize; y++)
    img->tby[y]=img->tby[y-1] + xsize;

  img->tbz[0]=0; xysize = xsize*ysize;
  for (z=1; z < zsize; z++)
    img->tbz[z]=img->tbz[z-1] + xysize;

  return(img);
}

void     iftDestroyImage(iftImage **img)
{
  iftImage *aux;

  aux = *img;
  if(aux != NULL){
    if (aux->val  != NULL)  free(aux->val); 
    if (aux->Cb  != NULL)  free(aux->Cb); 
    if (aux->Cr  != NULL)  free(aux->Cr); 
    if (aux->tby  != NULL)  free(aux->tby);
    if (aux->tbz  != NULL)  free(aux->tbz);
    free(aux);    
    *img = NULL;
  }
}

void iftCopyVoxelSize(iftImage *img1, iftImage *img2)
{
  img2->dx = img1->dx;
  img2->dy = img1->dy;
  img2->dz = img1->dz;
}

void iftUpdateMinMax(iftImage *img)
{
  img->minval = iftMinimumValue(img);
  img->maxval = iftMaximumValue(img);
}

char iftValidVoxel(iftImage *img, iftVoxel v)
{
  if ((v.x >= 0)&&(v.x < img->xsize)&&
      (v.y >= 0)&&(v.y < img->ysize)&&
      (v.z >= 0)&&(v.z < img->zsize))
    return(1);
  else
    return(0);
}

int iftMaximumValue(iftImage *img)
{
  int p;       

  img->maxval = INT_MIN;
  for (p=0; p < img->n; p++) 
    if (img->maxval < img->val[p])
      img->maxval = img->val[p];

  return(img->maxval); 
}

int iftMinimumValue(iftImage *img)
{
  int p;       

  img->minval = INT_MAX;
  for (p=0; p < img->n; p++) 
    if (img->minval > img->val[p])
      img->minval = img->val[p];

  return(img->minval); 
}

iftImage *iftReadImage(char *filename)
{
  iftImage  *img=NULL;	
  FILE    *fp=NULL;
  uchar   *data8=NULL;
  ushort  *data16=NULL;
  int     *data32=NULL;
  char    type[10];
  int     p,v,xsize,ysize,zsize;
  long    pos;

  fp = fopen(filename,"rb");
  if (fp == NULL){
    iftError(MSG2,"iftReadImage");
  }
  fscanf(fp,"%s\n",type);
  if((strcmp(type,"SCN")==0)){
    fscanf(fp,"%d %d %d\n",&xsize,&ysize,&zsize);
    img = iftCreateImage(xsize,ysize,zsize);
    fscanf(fp,"%f %f %f\n",&img->dx,&img->dy,&img->dz);
    fscanf(fp,"%d",&v);
    pos = ftell(fp);
    fseek(fp,(pos+1)*sizeof(char),SEEK_SET); 
    if (v==8){			
      data8  = iftAllocUCharArray(img->n);			
      fread(data8,sizeof(uchar),img->n,fp);
      for (p=0; p < img->n; p++) 
	img->val[p] = (int) data8[p];
      free(data8);
    } else 
      if (v==16) {
	data16 = iftAllocUShortArray(img->n);
	fread(data16,sizeof(ushort),img->n,fp);
	for (p=0; p < img->n; p++)
	  img->val[p] = (int) data16[p];
	free(data16);
      }else
	if (v==32) {
	  data32 = iftAllocIntArray(img->n);
	  fread(data32,sizeof(int),img->n,fp);
	  for (p=0; p < img->n; p++)
	    img->val[p] = data32[p];
	  free(data32);
	} else {
	  iftError("Input scene must be 8, 16, or 32 bit","iftReadImage");
	}
    img->minval = iftMinimumValue(img);
    img->maxval = iftMaximumValue(img);    
  }else{
    iftError("Invalid file type","iftReadImage");
  }

  fclose(fp);
  return(img);
}

void iftWriteImage(iftImage *img, char *filename) 
{
  FILE *fp=NULL;
  int   p;
  uchar  *data8 =NULL;
  ushort *data16=NULL;
  int    *data32=NULL;

  img->minval = iftMinimumValue(img);
  if (img->minval < 0)
    iftError("Cannot write image with negative values","iftWriteImage");

  fp = fopen(filename,"wb"); 
  if (fp == NULL) 
    iftError(MSG2,"iftWriteImage");

  fprintf(fp,"SCN\n");
  fprintf(fp,"%d %d %d\n",img->xsize,img->ysize,img->zsize);
  fprintf(fp,"%f %f %f\n",img->dx,img->dy,img->dz);

  img->maxval = iftMaximumValue(img);

  if (img->maxval < 256) {
    fprintf(fp,"%d\n",8);
    data8 = iftAllocUCharArray(img->n);
    for (p=0; p < img->n; p++) 
      data8[p] = (uchar) img->val[p];
    fwrite(data8,sizeof(uchar),img->n,fp);
    free(data8);
  } else 
    if (img->maxval < 65536) {
      fprintf(fp,"%d\n",16);
      data16 = iftAllocUShortArray(img->n);
      for (p=0; p < img->n; p++)
	data16[p] = (ushort) img->val[p];
      fwrite(data16,sizeof(ushort),img->n,fp);
      free(data16);
    } else
      if (img->maxval < INT_MAX) {
	fprintf(fp,"%d\n",32);
	data32 = iftAllocIntArray(img->n);
	for (p=0; p < img->n; p++)
	  data32[p] =  img->val[p];
	fwrite(data32,sizeof(int),img->n,fp);
	free(data32);
      } 

  fclose(fp);
}

iftImage *iftReadImageP5(char *filename)
{
  iftImage  *img=NULL;	
  FILE    *fp=NULL;
  uchar   *data8=NULL;
  char    type[10];
  int     p,v,xsize,ysize,zsize;
  long    pos;

  fp = fopen(filename,"rb");
  if (fp == NULL){
    iftError(MSG2,"iftReadImageP5");
  }

  fscanf(fp,"%s\n",type);
  if((strcmp(type,"P5")==0)){

    // assuming no # symbols with comments. Remove those lines from
    // the image file.

    fscanf(fp,"%d %d\n",&xsize,&ysize); zsize = 1;
    img = iftCreateImage(xsize,ysize,zsize);
    img->dz=0.0;
    fscanf(fp,"%d",&v);
    pos = ftell(fp);
    fseek(fp,(pos+1)*sizeof(char),SEEK_SET); 
    if (v==255){			
      data8  = iftAllocUCharArray(img->n);			
      fread(data8,sizeof(uchar),img->n,fp);
      for (p=0; p < img->n; p++) 
	img->val[p] = (int) data8[p];
      free(data8);
    } else {
      iftError("Invalid maximum value","iftReadImageP5");
    }
    img->maxval = iftMaximumValue(img);
    img->minval = iftMinimumValue(img);
  }else{
    iftError("Invalid image type","iftReadImageP5");
  }

  fclose(fp);
  return(img);
}

void iftWriteImageP5(iftImage *img, char *filename) 
{
  FILE *fp=NULL;
  int   p;
  uchar  *data8 =NULL;

  fp = fopen(filename,"wb"); 
  if (fp == NULL) 
    iftError(MSG2,"iftWriteImageP5");

  fprintf(fp,"P5\n");
  fprintf(fp,"%d %d\n",img->xsize,img->ysize);

  img->maxval = iftMaximumValue(img);
  img->minval = iftMinimumValue(img);

  if ((img->maxval < 256)&&(img->minval>=0)) {
    fprintf(fp,"%d\n",255);
    data8 = iftAllocUCharArray(img->n);
    for (p=0; p < img->n; p++) 
      data8[p] = (uchar) img->val[p];
    fwrite(data8,sizeof(uchar),img->n,fp);
    free(data8);
  } 
  else {
    iftError("Cannot write image as P5","iftWriteImageP5");
  }
  fclose(fp);
}

iftImage *iftReadImageP6(char *filename)
{
  iftImage  *img=NULL;	
  FILE    *fp=NULL;
  char    type[10];
  int     p,v,xsize,ysize,zsize;
  long    pos;
  iftColor RGB,YCbCr;

  fp = fopen(filename,"rb");
  if (fp == NULL){
    iftError(MSG2,"iftReadImageP6");
  }

  fscanf(fp,"%s\n",type);
  if((strcmp(type,"P6")==0)){

    // assuming no # symbols with comments. Remove those lines from
    // the image file.

    fscanf(fp,"%d %d\n",&xsize,&ysize); zsize = 1;
    img = iftCreateImage(xsize,ysize,zsize);
    img->Cb = iftAllocUShortArray(img->n);
    img->Cr = iftAllocUShortArray(img->n);
    img->dz=0.0;
    fscanf(fp,"%d",&v);
    pos = ftell(fp);
    fseek(fp,(pos+1)*sizeof(char),SEEK_SET); 
    if (v==255){			
      for (p=0; p < img->n; p++){	
	RGB.val[0] = fgetc(fp);
	RGB.val[1] = fgetc(fp);
	RGB.val[2] = fgetc(fp);
	YCbCr      = iftRGBtoYCbCr(RGB);
	img->val[p]=YCbCr.val[0];
	img->Cb[p] =(ushort)YCbCr.val[1];
	img->Cr[p] =(ushort)YCbCr.val[2];
      }
    } else {
      iftError("Invalid maximum value","iftReadImageP6");
    }
    img->maxval = iftMaximumValue(img);
    img->minval = iftMinimumValue(img);
  }else{
    iftError("Invalid image type","iftReadImageP6");
  }

  fclose(fp);
  return(img);
}

void iftWriteImageP6(iftImage *img, char *filename) 
{
  FILE *fp=NULL;
  int   p;
  iftColor YCbCr,RGB;

  fp = fopen(filename,"wb"); 
  if (fp == NULL) 
    iftError(MSG2,"iftWriteImageP6");

  fprintf(fp,"P6\n");
  fprintf(fp,"%d %d\n",img->xsize,img->ysize);

  img->maxval = iftMaximumValue(img);
  img->minval = iftMinimumValue(img);

  if ((img->maxval < 256)&&(img->minval>=0)) {
    fprintf(fp,"%d\n",255);
    for (p=0; p < img->n; p++) {     
      YCbCr.val[0] = img->val[p];
      YCbCr.val[1] = (int)img->Cb[p];
      YCbCr.val[2] = (int)img->Cr[p];
      RGB = iftYCbCrtoRGB(YCbCr);
      fputc(((uchar)RGB.val[0]),fp);
      fputc(((uchar)RGB.val[1]),fp);
      fputc(((uchar)RGB.val[2]),fp);	      
    }
  }else {
    iftError("Cannot write image as P6","iftWriteImageP6");
  }
  fclose(fp);
}

iftImage *iftReadImageP2(char *filename)
{
  iftImage  *img=NULL;	
  FILE    *fp=NULL;
  char    type[10];
  int     p,v,xsize,ysize,zsize;
  long    pos;

  fp = fopen(filename,"r");
  if (fp == NULL){
    iftError(MSG2,"iftReadImageP2");
  }

  fscanf(fp,"%s\n",type);
  if((strcmp(type,"P2")==0)){

    // assuming no # symbols with comments. Remove those lines from
    // the image file.

    fscanf(fp,"%d %d\n",&xsize,&ysize); zsize = 1;
    img = iftCreateImage(xsize,ysize,zsize);
    img->dz=0.0;
    fscanf(fp,"%d",&v);
    pos = ftell(fp);
    fseek(fp,(pos+1)*sizeof(char),SEEK_SET); 
    for (p=0; p < img->n; p++) 
      fscanf(fp,"%d",&img->val[p]);
    img->maxval = iftMaximumValue(img);
    img->minval = iftMinimumValue(img);
  }else{
    iftError("Invalid image type","iftReadImageP2");
  }

  fclose(fp);
  return(img);
}

void iftWriteImageP2(iftImage *img, char *filename) 
{
  FILE *fp=NULL;
  int   p;

  fp = fopen(filename,"w"); 
  if (fp == NULL) 
    iftError(MSG2,"iftWriteImageP2");

  fprintf(fp,"P2\n");
  fprintf(fp,"%d %d\n",img->xsize,img->ysize);

  img->maxval = iftMaximumValue(img);
  img->minval = iftMinimumValue(img);

  fprintf(fp,"%d\n",img->maxval);
  for (p=0; p < img->n; p++) 
    fprintf(fp,"%d ",img->val[p]);

  fclose(fp);
}

iftImage *iftRemoveROI(iftImage *img, iftImage *bin, iftVoxel *pos)
{
  int p,q;
  iftVoxel uo,uf,u;
  iftImage *roi=NULL;
  
  if ((img->xsize!=bin->xsize)||
      (img->ysize!=bin->ysize)||
      (img->zsize!=bin->zsize)){
    iftError("Images must have the same domain","iftRemoveROI");
  }

  uo.x = uo.y = uo.z = INT_MAX;
  uf.x = uf.y = uf.z = INT_MIN;
  for (u.z=0; u.z < img->zsize; u.z++)
    for (u.y=0; u.y < img->ysize; u.y++)
      for (u.x=0; u.x < img->xsize; u.x++){
	p = iftGetVoxelIndex(img,u);
	if (bin->val[p]!=0){
	  if (u.x < uo.x) uo.x = u.x;
	  if (u.y < uo.y) uo.y = u.y;
	  if (u.z < uo.z) uo.z = u.z;
	  if (u.x > uf.x) uf.x = u.x;
	  if (u.y > uf.y) uf.y = u.y;
	  if (u.z > uf.z) uf.z = u.z;
	}
      }

  roi = iftCreateImage(uf.x-uo.x+1,uf.y-uo.y+1,uf.z-uo.z+1);

  q = 0;
  for (u.z=uo.z; u.z <= uf.z; u.z++)
    for (u.y=uo.y; u.y <= uf.y; u.y++)
      for (u.x=uo.x; u.x <= uf.x; u.x++){
	p = iftGetVoxelIndex(img,u);
	roi->val[q]=img->val[p];
	q++;
      }

  *pos = uo;

  return(roi);
}

iftImage  *iftInsertROI(iftImage *img, iftImage *bin, iftImage *roi, iftVoxel  pos)
{
  iftImage *nimg=iftCopyImage(img);
  int p,q;
  iftVoxel u,v;

  if (!iftValidVoxel(img,pos))
    iftError("Invalid position for insertion","iftInsertROI");

  if ((roi->xsize >= img->xsize) ||
      (roi->ysize >= img->ysize) ||
      (roi->zsize >= img->zsize))
    iftError("ROI cannot be inserted in the image","iftInsertROI");

  for (u.z=0; u.z < roi->zsize; u.z++)
    for (u.y=0; u.y < roi->ysize; u.y++)
      for (u.x=0; u.x < roi->xsize; u.x++){
	v.x = u.x + pos.x;
	v.y = u.y + pos.y;
	v.z = u.z + pos.z;
	if (iftValidVoxel(img,v)){
	  p = iftGetVoxelIndex(roi,u);
	  q = iftGetVoxelIndex(img,v);
	  if (bin->val[q]!=0)
	    nimg->val[q] = roi->val[p];
	}
      }

  
  return(nimg);
}

iftImage  *iftCopyImage(iftImage *img)
{
    iftImage *imgc=iftCreateImage(img->xsize,img->ysize,img->zsize);

    iftCopyVoxelSize(img,imgc);

    memcpy(imgc->val,img->val,img->n*sizeof(int));

    if (img->Cb != NULL) {
      imgc->Cb = iftAllocUShortArray(img->n);
      memcpy(imgc->Cb,img->Cb,img->n*sizeof(ushort));
    }

    if (img->Cr != NULL) {
      imgc->Cr = iftAllocUShortArray(img->n);
      memcpy(imgc->Cr,img->Cr,img->n*sizeof(ushort));
    }

    return(imgc);
}


iftImage  *iftCreateCuboid(int xsize, int ysize, int zsize)
{
  iftImage *img=iftCreateImage(xsize,ysize,zsize);
  iftVoxel u,uo,uf;
  int p;

  uo.x = (int)(0.1*xsize); uo.y = (int)(0.1*ysize); uo.z = (int)(0.1*zsize);
  uf.x = (int)(0.9*xsize); uf.y = (int)(0.9*ysize); uf.z = (int)(0.9*zsize);
  
  for (u.z=uo.z; u.z <= uf.z; u.z++) 
    for (u.y=uo.y; u.y <= uf.y; u.y++) 
      for (u.x=uo.x; u.x <= uf.x; u.x++){
	p = iftGetVoxelIndex(img,u);
	img->val[p]=255;
      }

  return(img);

}


char iftAdjacentVoxels(iftImage *img, iftAdjRel *A, iftVoxel u, iftVoxel v)
{
  int i;

  for (i=0; i < A->n; i++) {
    if ((A->dx[i]==(v.x-u.x))&&
	(A->dy[i]==(v.y-u.y))&&
	((A->dz[i]==(v.z-u.z))))
      return 1;
  }

  return 0;
}

iftImage *iftCSVtoImage(char *filename)
{
  char      basename[100],ext[10],*pos,newfilename[150];
  iftImage  *img=NULL;
  int       len,p;
  iftVoxel  um,u;
  FILE     *fp=NULL;

  pos = strrchr(filename,'.') + 1;
  sscanf(pos,"%s",ext);
  
  if (strcmp(ext,"csv")==0){
    len = strlen(filename);
    strncpy(basename,filename,len-4);
    basename[len-4]='\0';
    sprintf(newfilename,"%s.scn",basename);
    fp = fopen(filename,"r");
    um.x=um.y=um.z=INT_MIN;
    while (!feof(fp)){
      fscanf(fp,"%d,%d,%d",&u.x,&u.y,&u.z);
      if (u.x > um.x) um.x = u.x;
      if (u.y > um.y) um.y = u.y;
      if (u.z > um.z) um.z = u.z;
    }
    fclose(fp);
    img = iftCreateImage(um.x+10,um.y+10,um.z+10);
    fp = fopen(filename,"r");
    while (!feof(fp)){
      fscanf(fp,"%d,%d,%d",&u.x,&u.y,&u.z);
      u.x=u.x+5; u.y=u.y+5; u.z=u.z+5;
      p = iftGetVoxelIndex(img,u);
      img->val[p]=255;
    }
    fclose(fp);
    iftWriteImage(img,newfilename);
  }else
    iftError(MSG2,"iftCSVtoImage");

  return(img);
}

iftImage *iftAddFrame(iftImage *img, int sz, int value)
{
  iftImage *fimg;
  int y, z,*dst,*src,nbytes,offset1, offset2;
  
  fimg = iftCreateImage(img->xsize+(2*sz),img->ysize+(2*sz), img->zsize+(2*sz));
  fimg->dx = img->dx;
  fimg->dy = img->dy;
  fimg->dz = img->dz;

  iftSetImage(fimg,value);
  nbytes = sizeof(int)*img->xsize;  
  offset1 = 0;
  offset2 = sz+ fimg->tby[sz] + fimg->tbz[sz];
  
  for(z=0; z<img->zsize; z++){
    src = img->val+offset1;
    dst = fimg->val+offset2;
    for (y=0; y<img->ysize; y++){
      memcpy(dst,src,nbytes);
      src += img->xsize;
      dst += fimg->xsize;
    }
    offset1 += img->xsize*img->ysize;
    offset2 += fimg->xsize*fimg->ysize;
  }
  
  return(fimg);
}

iftImage *iftRemFrame(iftImage *fimg, int sz) {
  iftImage *img;
  int y,z,*dst,*src,nbytes,offset;

  img = iftCreateImage(fimg->xsize-(2*sz),fimg->ysize-(2*sz),fimg->zsize-(2*sz));
  img->dx = fimg->dx;
  img->dy = fimg->dy;
  img->dz = fimg->dz;

  nbytes = sizeof(int)*img->xsize;  
  offset = sz + fimg->tby[sz] + fimg->tbz[sz];
  src = fimg->val+offset;
  dst = img->val;
  for (z=0; z < img->zsize; z++,src+=2*sz*fimg->xsize) {
    for (y=0; y < img->ysize; y++,src+=fimg->xsize,dst+=img->xsize){
      memcpy(dst,src,nbytes);
    }
  }
  return(img);
}

void iftSetImage(iftImage *img, int value)
{ 
  int i,n;
  n = img->xsize*img->ysize*img->zsize;
  for (i=0; i < n; i++){
    img->val[i]=value;
  }
}



iftImage *iftGetSlice(iftImage *img, iftPlane *pl, int xsize, int ysize) 
{ 
  iftImage *slice;
  int p,q;
  iftVoxel u,v;
  iftPoint P1,P2;
  iftMatrix *A;

  slice = iftCreateImage(xsize,ysize,1);
  A     = iftRotationMatrixToAlignZWithVector(pl->normal);

  u.z = 0;
  for (u.y=0; u.y < ysize; u.y++)
    for (u.x=0; u.x < xsize; u.x++){

      /* Consider the visualization plane initially placed in parallel
	 to the XY plane with z=0 and origin at the center of the
	 xsize x ysize region. This region must be first centered at
	 the origin of the XYZ coordinate system. After that, compute
	 P1 as the coordinates of pixel u in that region. */

      p = iftGetVoxelIndex(slice,u);
      P1.x = u.x - xsize/2.0;
      P1.y = u.y - ysize/2.0;
      P1.z = u.z;


      /* Rotate the region such that the Z axis (normal of the plane)
	 is aligned to the normal of the desired slicing plane */

      P2 = iftTransformPoint(A,P1);

      /* Translate P2 onto the position of the desired slicing plane */

      P2.x = P2.x + pl->pos.x;
      P2.y = P2.y + pl->pos.y;
      P2.z = P2.z + pl->pos.z;

      /* Get its voxel intensity in the scene (substitute ROUND by
	 trilinear interpolation) */

      v.x = ROUND(P2.x);
      v.y = ROUND(P2.y);
      v.z = ROUND(P2.z);

      if (iftValidVoxel(img,v)){
	q = iftGetVoxelIndex(img,v);
	slice->val[p]=img->val[q];
      }
    }

  iftDestroyMatrix(&A);
  
  return(slice);
}

iftVoxel iftGeometricCenterVoxel(iftImage *bin)
{
  iftVoxel u;
  iftPoint c;
  int p,n=0;

  c.x = c.y = c.z = 0.0;
  for (u.z=0; u.z < bin->zsize; u.z++) 
    for (u.y=0; u.y < bin->ysize; u.y++) 
      for (u.x=0; u.x < bin->xsize; u.x++) {
	p = iftGetVoxelIndex(bin,u);
	if (bin->val[p]!=0){
	  c.x += u.x;
	  c.y += u.y;
	  c.z += u.z;
	  n++;
	}
      }
  if (n==0)
    iftError("Empty image","iftGeometricCenter");

  u.x = ROUND(c.x / n);
  u.y = ROUND(c.y / n);
  u.z = ROUND(c.z / n);
  
  return(u);
}

iftImage *iftCropObjects(iftImage *bin)
{
  int p,q,xsize,ysize,zsize;
  iftVoxel u,v,c,min,max;
  iftImage *mbb;

  min.x = min.y = min.z = INT_MAX;
  max.x = max.y = max.z = INT_MIN;
  for (u.z=0; u.z < bin->zsize; u.z++)
    for (u.y=0; u.y < bin->ysize; u.y++)
      for (u.x=0; u.x < bin->xsize; u.x++){
	p = iftGetVoxelIndex(bin,u);
	if (bin->val[p]!=0){
	  if (u.x < min.x) min.x = u.x;
	  if (u.y < min.y) min.y = u.y;
	  if (u.z < min.z) min.z = u.z;
	  if (u.x > max.x) max.x = u.x;
	  if (u.y > max.y) max.y = u.y;
	  if (u.z > max.z) max.z = u.z;	  
	}
      }
  xsize = max.x - min.x + 1;
  ysize = max.y - min.y + 1;
  zsize = max.z - min.z + 1;
  c = iftGeometricCenterVoxel(bin);

  mbb   = iftCreateImage(xsize,ysize,zsize);
  q=0;
  for (u.z=c.z-zsize/2,v.z=0; u.z <= c.z+zsize/2; u.z++,v.z++)
    for (u.y=c.y-ysize/2,v.y=0; u.y <= c.y+ysize/2; u.y++,v.y++)
      for (u.x=c.x-xsize/2,v.x=0; u.x <= c.x+xsize/2; u.x++,v.x++)
	if ((iftValidVoxel(bin,u))&&(iftValidVoxel(mbb,v))){
	  p = iftGetVoxelIndex(bin,u);
	  q = iftGetVoxelIndex(mbb,v);
	  mbb->val[q]=bin->val[p];
	}

  return(mbb);
}

iftPoint iftGeometricCenter(iftImage *bin)
{
  iftVoxel u;
  iftPoint c;
  int p;
  unsigned long n=0;

  c.x = c.y = c.z = 0.0;
  for (u.z=0; u.z < bin->zsize; u.z++) 
    for (u.y=0; u.y < bin->ysize; u.y++) 
      for (u.x=0; u.x < bin->xsize; u.x++) {
	p = iftGetVoxelIndex(bin,u);
	if (bin->val[p]!=0){
	  c.x += u.x;
	  c.y += u.y;
	  c.z += u.z;
	  n++;
	}
      }
  if (n==0)
    iftError("Empty image","iftGeometricCenter");

  c.x /= n;
  c.y /= n;
  c.z /= n;

  return(c);
}

iftImage *iftResliceImage(iftImage *img, iftPlane *pl, int xsize, int ysize, int zsize)
{
  iftImage *rimg,*aux;
  iftPoint  P1,P0;
  iftVoxel  u;
  int       i,xysize,offset;

  /* Initialize output scene */

  rimg   = iftCreateImage(xsize,ysize,zsize);
  iftCopyVoxelSize(img,rimg);

  /* Starting at zsize/2 slices before, reslice the scene until zsize/2 slices
     after its center along the direction of v. */

  xysize = xsize*ysize;

  P0.x   = pl->pos.x - (zsize/2.0)*pl->normal.x;
  P0.y   = pl->pos.y - (zsize/2.0)*pl->normal.y;
  P0.z   = pl->pos.z - (zsize/2.0)*pl->normal.z; 

  for (i=0,offset=0; i < zsize; i++,offset+=xysize) {
    P1.x = P0.x + i*pl->normal.x; 
    P1.y = P0.y + i*pl->normal.y;
    P1.z = P0.z + i*pl->normal.z;
    u.x  = ROUND(P1.x);
    u.y  = ROUND(P1.y);
    u.z  = ROUND(P1.z);
    if (iftValidVoxel(img,u)){
      iftSetPlanePos(pl,P1.x,P1.y,P1.z);
      aux  = iftGetSlice(img,pl,xsize,ysize);
      memcpy(rimg->val+offset,aux->val,xysize*sizeof(int));
      iftDestroyImage(&aux);
    }
  }
  

  return(rimg);
}

iftImage *iftCropImage(iftImage *img, iftVoxel uo, iftVoxel uf)
{
  iftVoxel u,v;
  int p, q, xsize, ysize, zsize;
  iftImage *crop;

  xsize = uf.x - uo.x + 1;
  ysize = uf.y - uo.y + 1;
  zsize = uf.z - uo.z + 1;
  if ((xsize > img->xsize)||(ysize > img->ysize)||(zsize > img->zsize))
    iftError("Invalid crop region","iftCropImage");
  
  crop = iftCreateImage(xsize,ysize,zsize);
  if (img->Cb != NULL) crop->Cb = iftAllocUShortArray(crop->n);
  if (img->Cr != NULL) crop->Cr = iftAllocUShortArray(crop->n);

  for (u.z=uo.z, v.z=0; u.z <= uf.z; u.z++, v.z++)
    for (u.y=uo.y, v.y=0; u.y <= uf.y; u.y++, v.y++)
      for (u.x=uo.x, v.x=0; u.x <= uf.x; u.x++, v.x++){
	p   = iftGetVoxelIndex(img,u);
	q   = iftGetVoxelIndex(crop,v);
	crop->val[q]=img->val[p];
	if (img->Cb != NULL) crop->Cb[q] = img->Cb[p];
	if (img->Cr != NULL) crop->Cr[q] = img->Cr[p];	
      }

  return(crop);
}

iftImage *iftImageGradient(iftImage *img, iftAdjRel *A)
{
  float   dist,gx,gy,gz, g, gmax;
  float   gxCb , gyCb , gzCb, gxCr , gyCr , gzCr;
  int     i,p,q;
  iftVoxel   u,v;
  
  iftImage  *grad=iftCreateImage(img->xsize,img->ysize,img->zsize);

  if ( (img->Cb == NULL) && (img->Cr == NULL) ){
    for (u.z=0; u.z < img->zsize; u.z++)
      for (u.y=0; u.y < img->ysize; u.y++)
		for (u.x=0; u.x < img->xsize; u.x++) {
		  p = iftGetVoxelIndex(img,u);
		  gx = gy = gz = 0.0;
		  for (i=1; i < A->n; i++) {
			v.x = u.x + A->dx[i];
			v.y = u.y + A->dy[i];
			v.z = u.z + A->dz[i];
			if (iftValidVoxel(img,v)){
			  q = iftGetVoxelIndex(img,v);
			  dist = img->val[q]-img->val[p];
			  gx += dist*A->dx[i];
			  gy  += dist*A->dy[i];
			  gz  += dist*A->dz[i];
			}
		  }
		  grad->val[p]=(int)sqrtf(gx*gx + gy*gy + gz*gz);
		  grad->dx = gx;
		  grad->dy = gy;
		  grad->dz = gz;
		}
  }else{ // colored image
    for (u.z=0; u.z < img->zsize; u.z++)
      for (u.y=0; u.y < img->ysize; u.y++)
	for (u.x=0; u.x < img->xsize; u.x++) {
	  p = iftGetVoxelIndex(img,u);
	  gx = gy = gz = 0.0;
	  gxCb = gyCb = gzCb = 0.0;
	  gxCr = gyCr = gzCr = 0.0;
	  for (i=1; i < A->n; i++) {
	    v.x = u.x + A->dx[i];
	    v.y = u.y + A->dy[i];
	    v.z = u.z + A->dz[i];
	    if (iftValidVoxel(img,v)){
	      q = iftGetVoxelIndex(img,v);
	      dist = img->val[q]-img->val[p];
	      gx  += dist*A->dx[i];
	      gy  += dist*A->dy[i];
	      gz  += dist*A->dz[i];
	      dist = img->Cb[q]-img->Cb[p];
	      gxCb  += dist*A->dx[i];
	      gyCb  += dist*A->dy[i];
	      gzCb  += dist*A->dz[i];
	      dist = img->Cr[q]-img->Cr[p];
	      gxCr  += dist*A->dx[i];
	      gyCr  += dist*A->dy[i];
	      gzCr  += dist*A->dz[i];
	    }
	  }
	  gmax = sqrtf(gx*gx + gy*gy + gz*gz);
	  g    = sqrtf(gxCb*gxCb + gyCb*gyCb + gzCb*gzCb);
	  if (g > gmax) 
	    gmax = g;
	  g    = sqrtf(gxCr*gxCr + gyCr*gyCr + gzCr*gzCr);
	  if (g > gmax) 
	    gmax = g;
	  grad->val[p] = (int)gmax;
	  grad->dx = gx;
	  grad->dy = gy;
	  grad->dz = gz;
	}
  }
  return(grad);
}

iftImage *iftImageGradientMagnitude(iftImage *img, iftAdjRel *A)
{
  float   dist,gx,gy,gz, g, gmax;
  float   gxCb , gyCb , gzCb, gxCr , gyCr , gzCr;
  int     i,p,q;
  iftVoxel   u,v;
  float   *mag =iftAllocFloatArray(A->n);
  iftImage  *grad=iftCreateImage(img->xsize,img->ysize,img->zsize);

  for (i=0; i < A->n; i++)
    mag[i]=sqrtf(A->dx[i]*A->dx[i]+A->dy[i]*A->dy[i]+A->dz[i]*A->dz[i]);
  
  if ( (img->Cb == NULL) && (img->Cr == NULL) ){
    for (u.z=0; u.z < img->zsize; u.z++)
      for (u.y=0; u.y < img->ysize; u.y++)
		for (u.x=0; u.x < img->xsize; u.x++) {
		  p = iftGetVoxelIndex(img,u);
		  gx = gy = gz = 0.0;
		  for (i=1; i < A->n; i++) {
			v.x = u.x + A->dx[i];
			v.y = u.y + A->dy[i];
			v.z = u.z + A->dz[i];
			if (iftValidVoxel(img,v)){
			  q = iftGetVoxelIndex(img,v);
			  dist = img->val[q]-img->val[p];
			  gx  += dist*A->dx[i]/mag[i];
			  gy  += dist*A->dy[i]/mag[i];
			  gz  += dist*A->dz[i]/mag[i];
			}
		  }
		  grad->val[p]=(int)sqrtf(gx*gx + gy*gy + gz*gz);
		  grad->dirx[p] = gx;
		  grad->diry[p] = gy;
		  grad->dirz[p] = gz;
		  //printf( "%.2f ", atan2(gy,gx)*180/PI );
		}
  }else{ // colored image
    for (u.z=0; u.z < img->zsize; u.z++)
      for (u.y=0; u.y < img->ysize; u.y++)
	for (u.x=0; u.x < img->xsize; u.x++) {
	  p = iftGetVoxelIndex(img,u);
	  gx = gy = gz = 0.0;
	  gxCb = gyCb = gzCb = 0.0;
	  gxCr = gyCr = gzCr = 0.0;
	  for (i=1; i < A->n; i++) {
	    v.x = u.x + A->dx[i];
	    v.y = u.y + A->dy[i];
	    v.z = u.z + A->dz[i];
	    if (iftValidVoxel(img,v)){
	      q = iftGetVoxelIndex(img,v);
	      dist = img->val[q]-img->val[p];
	      gx  += dist*A->dx[i]/mag[i];
	      gy  += dist*A->dy[i]/mag[i];
	      gz  += dist*A->dz[i]/mag[i];
	      dist = img->Cb[q]-img->Cb[p];
	      gxCb  += dist*A->dx[i]/mag[i];
	      gyCb  += dist*A->dy[i]/mag[i];
	      gzCb  += dist*A->dz[i]/mag[i];
	      dist = img->Cr[q]-img->Cr[p];
	      gxCr  += dist*A->dx[i]/mag[i];
	      gyCr  += dist*A->dy[i]/mag[i];
	      gzCr  += dist*A->dz[i]/mag[i];
	    }
	  }
	  gmax = sqrtf(gx*gx + gy*gy + gz*gz);
	  g    = sqrtf(gxCb*gxCb + gyCb*gyCb + gzCb*gzCb);
	  if (g > gmax) 
	    gmax = g;
	  g    = sqrtf(gxCr*gxCr + gyCr*gyCr + gzCr*gzCr);
	  if (g > gmax) 
	    gmax = g;
	  grad->val[p] = (int)gmax;
	}
  }

  free(mag);

  return(grad);
}


void iftGetDisplayRange(iftImage *img, int *lower, int *higher)
{
  int p, i, n;
  double *hist;

  iftMinimumValue(img);
  iftMaximumValue(img);
  n=img->maxval-img->minval+1;

  hist = iftAllocDoubleArray(n);

  /* Compute histogram */
  for (p=0; p < img->n; p++) 
    hist[img->val[p]+img->minval]++;
  
  /* Compute normalized and accumulated histogram */

  hist[0] /= img->n;
  for (i=1; i < n; i++) {
    hist[i] = hist[i]/img->n + hist[i-1];
  }

  /* Compute lower value */

  for (i=0; i < n; i++) 
    if (hist[i] > 0.020){      
      *lower = (i-img->minval);
      break;
    }

  for (i=n-1; i >= 0; i--) 
    if (hist[i] < 0.998){
      *higher = (i-img->minval);
      break;
    }

  free(hist);

}

iftVoxel iftGetVoxelCoord(iftImage *img, int p)
{
  iftVoxel u;

  u.x = iftGetXCoord(img,p);
  u.y = iftGetYCoord(img,p);
  u.z = iftGetZCoord(img,p);

  return(u);
}

iftVoxel iftGetAdjacentVoxel(iftAdjRel *A, iftVoxel u, int adj)
{
  iftVoxel v;

  v.x = u.x + A->dx[adj];
  v.y = u.y + A->dy[adj];
  v.z = u.z + A->dz[adj];

  return(v);
}

iftImage *iftImageCb(iftImage *img)
{
  iftImage *Cb=iftCreateImage(img->xsize,img->ysize,img->zsize);
  int p;

  if (img->Cb == NULL) 
    iftError("There is no color component","iftImageCb");

  for (p=0; p < img->n; p++) 
    Cb->val[p] = img->Cb[p];

  return(Cb);
}

iftImage *iftImageCr(iftImage *img)
{
  iftImage *Cr=iftCreateImage(img->xsize,img->ysize,img->zsize);
  int p;

  if (img->Cr == NULL) 
    iftError("There is no color component","iftImageCb");

  for (p=0; p < img->n; p++) 
    Cr->val[p] = img->Cr[p];

  return(Cr);
}

iftImage *iftImageRed(iftImage *img)
{
  iftImage *red=iftCreateImage(img->xsize,img->ysize,img->zsize);
  iftColor RGB,YCbCr;
  int p;

  if ((img->Cr == NULL)||(img->Cb == NULL)) 
    iftError("There are no color components","iftImageRed");

  for (p=0; p < img->n; p++) {
    YCbCr.val[0] = img->val[p];
    YCbCr.val[1] = img->Cb[p];
    YCbCr.val[2] = img->Cr[p];
    RGB = iftYCbCrtoRGB(YCbCr);
    red->val[p] = RGB.val[0];
  }

  return(red);
}

iftImage *iftImageGreen(iftImage *img)
{
  iftImage *green=iftCreateImage(img->xsize,img->ysize,img->zsize);
  iftColor RGB,YCbCr;
  int p;

  if ((img->Cr == NULL)||(img->Cb == NULL)) 
    iftError("There are no color components","iftImageGreen");

  for (p=0; p < img->n; p++) {
    YCbCr.val[0] = img->val[p];
    YCbCr.val[1] = img->Cb[p];
    YCbCr.val[2] = img->Cr[p];
    RGB = iftYCbCrtoRGB(YCbCr);
    green->val[p] = RGB.val[1];
  }

  return(green);
}

iftImage *iftImageBlue(iftImage *img)
{
  iftImage *blue=iftCreateImage(img->xsize,img->ysize,img->zsize);
  iftColor RGB,YCbCr;
  int p;

  if ((img->Cr == NULL)||(img->Cb == NULL)) 
    iftError("There are no color components","iftImageBlue");

  for (p=0; p < img->n; p++) {
    YCbCr.val[0] = img->val[p];
    YCbCr.val[1] = img->Cb[p];
    YCbCr.val[2] = img->Cr[p];
    RGB = iftYCbCrtoRGB(YCbCr);
    blue->val[p] = RGB.val[1];
  }

  return(blue);
}
