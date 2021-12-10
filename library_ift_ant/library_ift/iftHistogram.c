#include "iftHistogram.h"

iftHist  *iftCreateHistogram(int n){
  iftHist *H=(iftHist *)calloc(1,sizeof(iftHist ));
  
  H->values = (int *)iftAllocIntArray(n);
  H->bin  = n;

  return(H);
}

void      iftDestroyHistogram(iftHist **H){
  iftHist *aux=*H;
  
  aux = *H;
  if (aux != NULL){
    if (aux->values != NULL) free(aux->values);
    free(aux);
    *H = NULL;
  }  
}

iftHist  *iftRunHistogram(iftImage *img, int x1, int y1, int x2, int y2, int bin ){
	iftVoxel u;
	int p, n_bin;
	iftHist *H = iftCreateHistogram( bin );
	float grade;

	//for (u.z=0; u.z < img->zsize; u.z++)
    for (u.y=y1; u.y < y2; u.y++)
      for (u.x=x1; u.x < x2; u.x++){
		  p = iftGetVoxelIndex(img,u);
		  grade = (atan2(img->diry[p],img->dirx[p])*180/PI);
		  n_bin = grade/( 180.0 / bin );
		  H->values[ n_bin ] += img->val[p];
	  }

	return (H);
}

iftHist  *iftUnionHistogram( iftHist *conj_hist, int *ids, int n ){
  int i, j, mag = 0;
  iftHist *H=(iftHist *)calloc(1,sizeof(iftHist ));
  
  H->values = (int *)iftAllocIntArray( n * conj_hist->bin );
  H->bin  = n * conj_hist->bin;

  for( i = 0; i < n; ++i ){
	  for( j = 0; j < conj_hist[ ids[ i ] ].bin; ++j ){
		  H->values[ j + conj_hist[ ids[ i ] ].bin * i ] = conj_hist[ ids[ i ] ].values[ j ];
		  mag += conj_hist[ ids[ i ] ].values[ j ];
	  }
  }

  for( i = 0; i < H->bin; ++i )
	  H->values[ i ] /= mag;

  return(H);
}