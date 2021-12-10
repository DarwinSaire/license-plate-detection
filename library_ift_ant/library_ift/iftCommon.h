#ifndef IFT_COMMON_H_
#define IFT_COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include<sys\timeb.h>
//#include <sys/time.h>



/* 
 * Common data types
 */

//typedef struct timeval timer;

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

typedef struct ift_vector {
  float x,y,z;
} iftVector, iftPoint, iftScale;
typedef struct ift_voxel {
  int x,y,z;
} iftVoxel;

/* Fitness function used for optimization problems */
typedef float (*iftFitnessFunc)   (void *problem, float *theta); 

/** 
 * Common definitions 
 */

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define PI          3.1415926536
#define INTERIOR    0
#define EXTERIOR    1
#define BOTH        2
#define WHITE       0
#define GRAY        1
#define BLACK       2
#define NIL        -1
#define INCREASING  1
#define DECREASING  0
#define Epsilon     1E-05

/** 
 * Common operations 
 */

#ifndef MAX
#define MAX(x,y) (((x) > (y))?(x):(y))
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y))?(x):(y))
#endif

#define ROUND(x) ((x < 0)?(int)(x-0.5):(int)(x+0.5))
#define SIGN(x) ((x >= 0)?1:-1)

/** 
 * Common functions to allocate memory
 */

char   *iftAllocCharArray(int n);  
uchar  *iftAllocUCharArray(int n); 
short  *iftAllocShortArray(int n);
ushort *iftAllocUShortArray(int n);
uint   *iftAllocUIntArray(int n); 
int    *iftAllocIntArray(int n);  
float  *iftAllocFloatArray(int n);
double *iftAllocDoubleArray(int n);
long double *iftAllocLongDoubleArray(int n);

/** 
 * Error messages 
 */

#define MSG1  "Cannot allocate memory space"
#define MSG2  "Cannot open file"

/**
 *  Error message msg is printed in function func and the program
 *  exits abnormally.
 */
                     
void iftError(char *msg,char *func); 

/**
 *  Warning message msg is printed in function func and the program
 *  continues.
 */
                     
void iftWarning(char *msg,char *func); 

/**
 *  The contents of a and b are interchanged. 
 */

void iftSwitchValues(int *a, int *b); 
void iftSwitchFValues(float *a, float *b); 


/**
 * Returns a random integer number between low and high.
 */

int iftRandomInteger (int low, int high);

/**
 * Returns the distance between P0 and the line from P1 to P2, whose
 * size is P1P2
 */

float iftPointLineDist2D(iftVoxel P0, iftVoxel P1, iftVoxel P2, float P1P2);

/**
 * Returns the Euclidean distance between P1 and P2 in the xy plane
 */

float iftEuclDistXY(iftVoxel P1, iftVoxel P2);

/**
 * Returns initial time 
 */

//timer *iftTic(); 

/**
 * Returns final time 
 */

//timer *iftToc(); 

/**
 * Computes the difference in ms from the initial time to the final time
 */

//float iftCompTime(timer *tic, timer *toc); 

/** 
 * Generates seed for rand(), used in iftRandomInteger.
 */

void iftRandomSeed(); 

/**
 * Returns the factorial of a number or NIL in case of overflow
 */

long double iftFactorial(int n);

/**
 * Returns the limit to avoid overflow in factorial computation
 */

int iftFactorialLimit();

float *iftCopyFloatArray(float *u, int n);

#endif
