//
// Created by Alan Peixinho on 9/3/15.
//

#include <ift.h>

int main() {

    //read image form disk
    iftImage* img = iftReadImageByExt("lena.pgm");

    //create a new image with same size as the original image
    iftImage* newImg = iftCreateImage(img->xsize, img->ysize, img->zsize);

    iftVoxel v;

    v.z = 0;
    for (v.x = 1; v.x < newImg->xsize-1; ++v.x) {
        for (v.y = 1; v.y < newImg->ysize-1; ++v.y) {

            //convert the multidimensional coordinate to an unidimensional coordinate
            int p = iftGetVoxelIndex(newImg, v);

            iftVoxel u;
            u.z = 0;
            int s = 0;
            for (u.x = v.x-1; u.x <= v.x+1; ++u.x) {
                for (u.y = v.y-1; u.y <= v.y+1; ++u.y) {
                    int q = iftGetVoxelIndex(img, u);
                    s += img->val[q];
                }
            }

            newImg->val[p] = s/9;
        }
    }

    iftWriteImageP2(newImg, "nova_lena.pgm");

    iftDestroyImage(&img);
    iftDestroyImage(&newImg);

    return 0;
}