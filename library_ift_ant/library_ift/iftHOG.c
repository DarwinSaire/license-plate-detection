
#include "iftHOG.h"

iftHog  *iftCreateHog(iftImage *img, int icell, int iblock, int nbin){
  iftHog *Hog=(iftHog *)calloc(1,sizeof(iftHog ));
  
  Hog->cell = icell;
  Hog->block = iblock;

  Hog->nwidth = img->xsize / Hog->cell + ( img->xsize % Hog->cell ) ? 1 : 0;
  Hog->nheight = img->ysize / Hog->cell + ( img->ysize % Hog->cell ) ? 1 : 0;

  Hog->hist = (iftHist *)calloc( Hog->nwidth * Hog->nheight,sizeof(iftHist ));
  Hog->bin  = nbin;

  return(Hog);
}

void splitImage( iftImage *img, iftHog* Hog ){
	int w, h;
	for( w = 0; w < Hog->nwidth; ++w ){
		for( h = 0; h < Hog->nheight; ++h ){
			Hog->hist[ w + w * h ] = *iftRunHistogram( img, w * Hog->cell + 1, h * Hog->cell + 1, 
				MIN( img->xsize, ( w + 1 ) * Hog->cell ), MIN( img->ysize, ( h + 1 ) * Hog->cell ) );
		}
	}
}

iftHist *iftRunHog(iftImage *img, int icell, int iblock, int nbin){
	int i, j, k, xi, yi, x, y;
	iftHog *Hog = iftCreateHog(img,icell,iblock,nbin);
	splitImage(img,Hog);

//	iftHist *final_hist = iftCreateHistogram( Hog->bin * Hog->cell * Hog->block );

	/*for( x = 0;  x < Hog->nwidth; ++x ){
		for( y = 0; y < Hog->nheight; ++y ){
			for( i = 0; i < ( Hog->block / Hog->cell ) ; ++i ){
				for( j = 0; j < ( Hog->block / Hog->cell ); ++j ){
					xi = x + i;
					yi = y + j;
				}
			}
			//iftUnionHistogram(Hog->hist,5,Hog->block );
		}
	}*/
}