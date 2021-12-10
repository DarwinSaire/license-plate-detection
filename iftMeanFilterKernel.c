//
// Created by Alan Peixinho on 9/3/15.
//

#include "ift.h"

int main() {

    iftImage* img = iftReadImageByExt("lena.pgm");

    iftAdjRel* adj = iftRectangular(15, 15);

    //creates a uniform kernel (all neighbors with same weight --1--)
    //a kernel is an adjacency with a different weight (importance) for each neighbor.
    iftKernel* meanKernel = iftUniformKernel(adj);

    for (int i = 0; i < meanKernel->A->n; ++i) {
        meanKernel->weight[i]/=meanKernel->A->n;
    }

    iftImage* newImg = iftLinearFilter(img, meanKernel);

    iftWriteImageP2(newImg, "nova_lena.pgm");

    iftDestroyImage(&img);
    iftDestroyImage(&newImg);

}
