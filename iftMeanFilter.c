//
// Created by Alan Peixinho on 9/3/15.
//

#include "ift.h"

int main() {

    iftImage* img = iftReadImageByExt("lena.pgm");

    //creates a rectangular adjacency around the voxel
    iftAdjRel* adj = iftRectangular(25, 2);

    iftImage* newImage = iftCreateImage(img->xsize, img->ysize, img->zsize);

    for (int p = 0; p < img->n; ++p) {//foreach pixel in the image
        iftVoxel v = iftGetVoxelCoord(img, p);//gets the multidimensional coordinate using the unidimensional index
        int s = 0;
        for (int i = 0; i < adj->n; ++i) {//foreach neighbor voxel in the adjacency
            iftVoxel u = iftGetAdjacentVoxel(adj, v, i);
            if(iftValidVoxel(img, u)) {//check if it is a valid voxel (is it inside the image?)
                int q = iftGetVoxelIndex(img, u);
                s+= img->val[q];
            }
        }
        newImage->val[p] = s/adj->n;
    }

    iftWriteImageP2(newImage, "nova_lena.pgm");

    iftDestroyImage(&img);
    iftDestroyImage(&newImage);

}
