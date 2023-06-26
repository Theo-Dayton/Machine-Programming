/*
 *     ppmrotate.c
 *     by Theo Dayton and Thomson Brand, 10/15/2021
 *     Locality and the costs of loads and stores
 *
 *     Contains implementations for the ppmrotate function, whose purpose is to
 *     read in a file and write the resulting image to stdout after rotating
 */

#include "ppmrotate.h"
#include "mem.h"
typedef A2Methods_UArray2 A2;

struct Combination {
        A2Methods_T methods;
        void* newarr;
};

typedef struct Combination Combination;

void print(Combination helperStruct, A2 pixmap);
void initArray(A2 array, A2Methods_T methods);


/* 
 *  rotate90 function, which is used only when mapping to the array.
 *  rotates an image 90 degrees
*/
void rotate90(int col, int row, A2 array2b, void *elem, void *cl)
{
     A2Methods_T methods = ((Combination*)cl)->methods;
     A2 helperArr = ((Combination*)cl)->newarr;

    int height = methods->height(array2b);

    ((Pnm_rgb)methods->at(helperArr,height-row-1,col))->red = ((Pnm_rgb)elem)->red;
    ((Pnm_rgb)methods->at(helperArr,height-row-1,col))->blue = ((Pnm_rgb)elem)->blue;
    ((Pnm_rgb)methods->at(helperArr,height-row-1,col))->green = ((Pnm_rgb)elem)->green;

    (void) elem;
}

/* 
 *  rotate180 function, which is used only when mapping to the array.
 *  rotates an images 180 degrees
*/
void rotate180(int col, int row, A2 array2b, void *elem, void *cl)
{
     A2Methods_T methods = ((Combination*)cl)->methods;
     A2 helperArr = ((Combination*)cl)->newarr;

    int height = methods->height(array2b);
    int width = methods->width(array2b);

    ((Pnm_rgb)methods->at(helperArr,width - col - 1, height - row - 1))->red = ((Pnm_rgb)elem)->red;
    ((Pnm_rgb)methods->at(helperArr,width - col - 1, height - row - 1))->blue = ((Pnm_rgb)elem)->blue;
    ((Pnm_rgb)methods->at(helperArr,width - col - 1, height - row - 1))->green = ((Pnm_rgb)elem)->green;

    (void) elem;
}

/* 
 *  ppmrotate function, which takes in a pixmap, methods, map, and a rotate of type int.
 *  the function then rotates image based of the rotate int, and outputs result to stdout.
*/
void ppmrotate(Pnm_ppm pixmap, A2Methods_T methods, A2Methods_mapfun *map, int rotate, int width, int height)
{
    Combination helperStruct;
    helperStruct.methods = methods;

    if (rotate == 0) {
        Pnm_ppmwrite(stdout, pixmap);
    }
    else if (rotate == 90) {
        helperStruct.newarr = methods->new(methods->height(pixmap),methods->width(pixmap),methods->size(pixmap));
        initArray(helperStruct.newarr,methods);
        methods->map_default(pixmap->pixels,rotate90,&helperStruct);
        print(helperStruct, pixmap);
    }
    else if (rotate == 180) {
        helperStruct.newarr = methods->new(methods->width(pixmap),methods->height(pixmap),methods->size(pixmap));
        initArray(helperStruct.newarr,methods);
        methods->map_default(pixmap->pixels,rotate180,&helperStruct);
        print(helperStruct, pixmap);    
    }
    (void) map;
    (void) width;
    (void) height;
}

/* 
 *  initArray helper function which initializes an array given to it
*/
void initArray(A2 array, A2Methods_T methods) {
    for (int i = 0; i < methods->width(array); i++) {
        for (int j = 0; j < methods->height(array); j++) {
            Pnm_rgb element = malloc(sizeof(Pnm_rgb));

            unsigned test = 0;
            element->blue = test;
            element->red = test;
            element->green = test;
            element = methods->at(array, i, j);
        }
    }
}

/* 
 *  helper function print which prints out the resulting pixmap
*/
void print(Combination helperStruct, A2 originalPixmap) {
    Pnm_ppm pixmap = originalPixmap;
    helperStruct.methods->free(&pixmap->pixels);
    pixmap->pixels = helperStruct.newarr;
    pixmap->height = helperStruct.methods->height(helperStruct.newarr);
    pixmap->width = helperStruct.methods->width(helperStruct.newarr);
    Pnm_ppmwrite(stdout, pixmap);
}