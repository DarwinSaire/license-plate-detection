
// Created by Alan Peixinho on 9/3/15.
//
#include <ift.h>

int main() {
    iftImage* img = iftReadImageByExt("super.pgm");

    //creates a circular adjacency with radius 15
    iftAdjRel* adj = iftCircular(3.0);

    //apply erosion with circular adjacency
    iftImage* newImg = iftErode(img, adj);

    iftWriteImageP2(newImg, "novo_super.pgm");

    iftDestroyImage(&img);
    iftDestroyImage(&newImg);
    
    return 0;
}
