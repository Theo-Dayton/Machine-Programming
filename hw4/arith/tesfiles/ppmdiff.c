#ifndef PPMDIFF_H_INCLUDED
#define PPMDIFF_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include <stdbool.h>
#include <math.h>

// from here: https://stackoverflow.com/questions/3437404/min-and-max-in-c

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void ppmdiff(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Invalid amount of arguments \n");
        exit(1);
    }

    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    
    Pnm_ppm image1;
    Pnm_ppm image2;
    bool imageFlag = false;

    for (int i = 1; i < 3; i++) {
        FILE *infile;
        if (strcmp(argv[i],"-") != 0) {
            infile = fopen(argv[i], "r");
            if (imageFlag == false) {
                image1 = Pnm_ppmread(infile, methods);
                imageFlag = true;
            }
            else {
                image2 = Pnm_ppmread(infile, methods);
            }
            fclose(infile);
        }
        else {
            image2 = Pnm_ppmread(stdin, methods);
        }
    }

    assert(image1 && image2);

    int widthDiff = abs((int)(image1->width - image2->width));
    int heightDiff = abs((int)(image1->height - image2->height));

    if(widthDiff > 1 || heightDiff > 1){
        fprintf(stderr,"Error: width/height difference greater than 1\n");
        exit(1);
    }

    double sum = 0;

    float small_width = MIN(image1->width, image2->width);
    float small_height = MIN(image1->height, image2->height);

    void *pixel1;
    void *pixel2;
    for(int i = 0; i < small_width; i++){
        for(int j = 0; j < small_height; j++){
            pixel1 = methods->at(image1->pixels,i,j);
            pixel2 = methods->at(image2->pixels,i,j);

            double R1, R2, B1, B2, G1, G2;
            R1 = (double)((Pnm_rgb)pixel1)->red/image1->denominator;
            R2 = (double)((Pnm_rgb)pixel2)->red/image2->denominator;

            G1 = (double)((Pnm_rgb)pixel1)->green/image1->denominator;
            G2 = (double)((Pnm_rgb)pixel2)->green/image2->denominator;

            B1 = (double)((Pnm_rgb)pixel1)->blue/image1->denominator;
            B2 = (double)((Pnm_rgb)pixel2)->blue/image2->denominator;

            sum = sum + ((R1 - R2)*(R1 - R2) + (B1 - B2)*(B1 - B2) + (G1 - G2)*(G1 - G2));
        }
    }
    sum = sqrt(sum/(3*small_width*small_height));
    printf("%f\n",sum);
    
}

int main(int argc, char *argv[])
{
    ppmdiff(argc, argv);
}

#undef PPMDIFF_H_INCLUDED
#endif