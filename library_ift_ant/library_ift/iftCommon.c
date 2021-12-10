#include "iftCommon.h"

/*! \file
 * Author: Alexandre Falcao
 * Date: Aug 22, 2011
 * Last Update: Aug 22, 2011
 */

/*! \brief 
 * Allocates character memory array for n elements 
 */

char *iftAllocCharArray(int n)
{
  char *v=NULL;
  v = (char *) calloc(n,sizeof(char));
  if (v == NULL)
    iftError(MSG1,"iftAllocCharArray");
  return(v);
}

/*! \brief
 * Allocates unsigned character memory array for n elements. 
 */

uchar *iftAllocUCharArray(int n)
{
  uchar *v=NULL;
  v = (uchar *) calloc(n,sizeof(uchar));
  if (v == NULL)
    iftError(MSG1,"iftAllocUCharArray");
  return(v);
}

/*! \brief
 * Allocates short memory array for n elements. 
 */

short *iftAllocShortArray(int n)
{
  short *v=NULL;
  v = (short *) calloc(n,sizeof(short));
  if (v == NULL)
    iftError(MSG1,"iftAllocShortArray");
  return(v);
}

/*! \brief
 * Allocates unsigned short memory array for n elements. 
 */

ushort *iftAllocUShortArray(int n)
{
  ushort *v=NULL;
  v = (ushort *) calloc(n,sizeof(ushort));
  if (v == NULL)
    iftError(MSG1,"iftAllocUShortArray");
  return(v);
}

/*! \brief
 * Allocates integer memory array for n elements. 
 */

int *iftAllocIntArray(int n)
{
  int *v=NULL;
  v = (int *) calloc(n,sizeof(int));
  if (v == NULL)
    iftError(MSG1,"iftAllocIntArray");
  return(v);
}

/*! \brief
 * Allocates unsigned integer memory array for n elements. 
 */

uint *iftAllocUIntArray(int n)
{
  uint *v=NULL;
  v = (uint *) calloc(n,sizeof(uint));
  if (v == NULL)
    iftError(MSG1,"iftAllocUIntArray");
  return(v);
}


/*! \brief
 * Allocates float memory array for n elements. 
 */

float *iftAllocFloatArray(int n)
{
  float *v=NULL;
  v = (float *) calloc(n,sizeof(float));
  if (v == NULL)
    iftError(MSG1,"iftAllocFloatArray");
  return(v);
}

float *iftCopyFloatArray(float *u, int n)
{
    float *v=NULL;
    v = (float *) calloc(n,sizeof(float));
    if (v == NULL)
        iftError(MSG1,"iftCopyFloatArray");
    memcpy(v, u,n*sizeof(float));
    return v;
}

/*! \brief
 * Allocates double memory array for n elements. 
 */

double *iftAllocDoubleArray(int n)
{
  double *v=NULL;
  v = (double *) calloc(n,sizeof(double));
  if (v == NULL)
    iftError(MSG1,"iftAllocDoubleArray");
  return(v);
}

/*! \brief
 * Allocates long double memory array for n elements. 
 */

long double *iftAllocLongDoubleArray(int n)
{
  long double *v=NULL;
  v = (long double *) calloc(n,sizeof(long double));
  if (v == NULL)
    iftError(MSG1,"iftAllocLongDoubleArray");
  return(v);
}

/*! \brief 
 * Error message msg is printed in function func and the
 * program exits abnormally.
 */

void iftError(char *msg,char *func){ 
  fprintf(stderr,"Error:%s in %s\n",msg,func);
  exit(-1);
}

/*! \brief 
 * Warning message msg is printed in function func and the
 * program continues.
 */

void iftWarning(char *msg,char *func){ 
 fprintf(stdout,"Warning:%s in %s\n",msg,func);
}

/*! \brief 
 * Switches the contents of integer variables.
 */

void iftSwitchValues(int *a, int *b){ 
  int c;
  c  = *a;
  *a = *b;
  *b = c;
}

/*! \brief 
 * Switches the contents of float variables.
 */

void iftSwitchFValues(float *a, float *b){ 
  float c;
  c  = *a;
  *a = *b;
  *b = c;
}

/*! \brief 
 * Randomly selects an integer from low to high
 */

int iftRandomInteger (int low, int high){
  int k;
  double d;

  d = (double) rand () / ((double) RAND_MAX + 0.5);
  k =  MIN((int)(d * (double)(high - low + 1.0) ) + low, high);

  return k;
}

/*! \brief 
 * Returns the distance between P0 and the line from P1 to P2,
 * whose size is P1P2.
 */

float iftPointLineDist2D(iftVoxel P0, iftVoxel P1, iftVoxel P2, float P1P2)
{
  return(fabs((P2.x - P1.x)*(P1.y - P0.y) - (P1.x - P0.x)*(P2.y - P1.y))/P1P2);
}

/*! \brief 
 * Returns the Euclidean distance between P1 and P2 in the XY plane.
 */

float iftEuclDistXY(iftVoxel P1, iftVoxel P2)
{
  return(sqrtf((P2.y - P1.y)*(P2.y - P1.y) + (P2.x - P1.x)*(P2.x - P1.x)));
}

/*! \brief 
 * Returns the initial time 
 */

/*timer *iftTic(){ 
  timer *tic=NULL;
  tic = (timer *)malloc(sizeof(timer));
  gettimeofday(tic,NULL); 
  return(tic);
}*/

/*! \brief 
 * Returns the final time 
 */

/*timer *iftToc(){ 
  timer *toc=NULL;
  toc = (timer *)malloc(sizeof(timer));
  gettimeofday(toc,NULL);
  return(toc);
}*/

/*! \brief 
 * Returns the difference from the initial to the final times
 */

/*float iftCompTime(timer *tic, timer *toc) 
{ 
  float t=0.0;
  if ((tic!=NULL)&&(toc!=NULL)){
    t = (toc->tv_sec-tic->tv_sec)*1000.0 + 
      (toc->tv_usec-tic->tv_usec)*0.001;
    free(tic);free(toc);
  }
  return(t);
}*/

/*! \brief 
 * Generates a new seed for rand(), used in iftRandomInteger()
 */

void iftRandomSeed() 
{ 
  unsigned int t=0;
  /*timer *T=iftTic();

  if (T!=NULL){
    t = (unsigned int)(T->tv_usec*0.001);
    srand(t);
    free(T);
  }*/
}

/*! \brief 
 * Returns the factorial of a number or NIL in case of overflow 
 */

long double iftFactorial(int n)
{
  long double fact=1, maxval=1.0;
  int i;

  /* First find the maximum value of long double */

  maxval = powl(2.0,8*sizeof(long double)-1);

  for (i=1; i <= n; i++) {    
    if (fact >= (maxval/i)){ // overflow
      return(NIL);
    }    
    fact=fact*i;
  }   
  return(fact);
}

/*! \brief 
 * Returns the limit to avoid overflow in the factorial computation
 */

int iftFactorialLimit()
{
  long double fact=1.0, maxval=1.0;
  int i;

  /* First find the maximum value of long double */

  maxval = powl(2.0,8*sizeof(long double)-1);

  i = 1;
  while (fact < (maxval/i)){
    fact=fact*i;
    i++;
  }
  return(i-1);
}
