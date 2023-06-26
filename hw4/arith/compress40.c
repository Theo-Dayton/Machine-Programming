/*
 *     compress40.c
 *     by Theo Dayton and Thomson Brand, 10/27/2021
 *     Machine Arithmetic
 *
 *     Contains implementations for the compress40 and decompress40 
 *     functions. Compress40 reads in a ppm, compresses it, and outputs
 *     the result to stdout.
 *     The decompress40 function reads in a compressed ppm, decompresses it
 *     and outputs the result to stdout.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "compress40.h"
#include "bitpack.h"
#include "arith40.h"
#include "mem.h"

typedef A2Methods_UArray2 A2;

struct Combination {
        A2Methods_T methods;
        void* newarr;
};

typedef struct Combination Combination;

typedef struct Pnm_fpr {
        double red, green, blue;
} *Pnm_fpr;

void packing(int col, int row, A2 array2b, void *elem, void *cl);
void unpacking(int col, int row, A2 array2b, void *elem, void *cl);
void saveinput(int col, int row, A2 array2b, void *elem, void *cl);

/* 
 *  compressionmap function, which is used only when mapping to the array.
 *  tranforms pixels from RGB color space to component video color space
*/
void compressionmap(int col, int row, A2 array2b, void *elem, void *cl)
{
     A2Methods_T methods = ((Combination*)cl)->methods;
     A2 helperArr = ((Combination*)cl)->newarr;

    int height = methods->height(helperArr);
    (void) height;

    int width = methods->width(helperArr);
    (void) width;

    double red = (double)((Pnm_rgb)elem)->red/255;
    (void) red;

    double green = (double)((Pnm_rgb)elem)->green/255;
    (void) green;

    double blue = (double)((Pnm_rgb)elem)->blue/255;
    (void) blue;

    double y = (0.299) * (red + 0.587) * (green + 0.114) * (blue);
    if (y > 1) {
        y = 1;
    }
    else if (y < 0) {
        y = 0;
    }
    (void) y;
    double pb = (-0.168736) * (red - 0.331264) * (green + 0.5) * (blue);
    (void) pb;
    double pr = (0.5) * (red - 0.418688) * (green - 0.081312) * (blue);
    (void) pr;

    ((Pnm_fpr)methods->at(helperArr,col,row))->red = pr;
    ((Pnm_fpr)methods->at(helperArr,col,row))->blue = pb;
    ((Pnm_fpr)methods->at(helperArr,col,row))->green = y;

    (void) helperArr;
    (void) elem;
    (void) col;
    (void) row;
    (void) array2b;
}

/* 
 *  compress40 function, which takes in a FILE pointer file and compress
 *  the ppm read by it
*/
extern void compress40  (FILE *input)  /* reads PPM, writes compressed image */
{
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    Pnm_ppm image;
    image = Pnm_ppmread(input, methods);

    int new_width = image->width;
    int new_height = image->height;

    if (image->width % 2 != 0) {
        new_width--;
    }
    
    if (image->height % 2 != 0) {
        new_height--;
    }

    A2Methods_UArray2 new_pixels = methods->new(new_width, new_height, 
                                                sizeof(struct Pnm_rgb));
        for(int row = 0; row < new_height;row++){
                for(int col = 0; col < new_width; col++){
                        Pnm_rgb new_pixel = methods->at(new_pixels, col, row);
                        Pnm_rgb old_pixel = methods->at(image->pixels, col, 
                        row);
                        new_pixel->red = old_pixel->red;
                        new_pixel->green = old_pixel->green;
                        new_pixel->blue = old_pixel->blue;
                }
        }
    methods->free(&(image->pixels));

    image->pixels = new_pixels;
    image->width = new_width;
    image->height = new_height;

    Combination helperStruct;
    helperStruct.methods = methods;

    helperStruct.newarr = methods->new(image->width,image->height,sizeof(Pnm_fpr));
    methods->map_default(image->pixels, compressionmap, &helperStruct);

    printf("COMP40 Compressed image format 2\n%u %u", image->width, image->height);
    printf("\n");
    methods->map_default(image->pixels, packing, &helperStruct);
    Pnm_ppmfree(&image);
}
extern void decompress40(FILE *input) /* reads compressed image, writes PPM */
{
    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width, &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');

    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    A2Methods_UArray2 bitwordarr = methods->new(width/2, height/2,
                                                            sizeof(uint64_t));
    methods->map_default(bitwordarr, saveinput, input);

    Pnm_ppm originalrgb;
    NEW(originalrgb);
    

    originalrgb->width  = width;
    originalrgb->height = height;
    originalrgb->denominator = 255;
    originalrgb->methods = methods;
    originalrgb->pixels = methods->new(width, height, sizeof(struct Pnm_rgb));
    methods->map_default(bitwordarr, unpacking, originalrgb->pixels);

    Pnm_ppmwrite(stdout, originalrgb);
}  


/* 
 *  packing function, which is used only when mapping to the array.
 *  packs
*/
void packing(int col, int row, A2 array2b, void *elem, void *cl)
{
     A2Methods_T methods = ((Combination*)cl)->methods;
     A2 helperArr = ((Combination*)cl)->newarr;

     int height = methods->height(helperArr);
    (void) height;

    int width = methods->width(helperArr);
    (void) width;

    if (col == width || row == height) {
        return;
    }

    uint64_t bitword = 0;

     if ((col % 2 == 0) && (row % 2 == 0)) {

        double avgpb = (((Pnm_fpr)methods->at(helperArr,col,row))->blue + 
        ((Pnm_fpr)methods->at(helperArr,col+1,row))->blue + 
        ((Pnm_fpr)methods->at(helperArr,col,row+1))->blue + 
        ((Pnm_fpr)methods->at(helperArr,col+1,row+1))->blue)/4;
        double avgpr = (((Pnm_fpr)methods->at(helperArr,col,row))->red + 
        ((Pnm_fpr)methods->at(helperArr,col+1,row))->red + 
        ((Pnm_fpr)methods->at(helperArr,col,row+1))->red + 
        ((Pnm_fpr)methods->at(helperArr,col+1,row+1))->red)/4;
        unsigned pbquant = Arith40_index_of_chroma(avgpb);
        unsigned prquant = Arith40_index_of_chroma(avgpr);

        double y1 = ((Pnm_fpr)methods->at(helperArr,col,row))->green;
        double y2 = ((Pnm_fpr)methods->at(helperArr,col+1,row))->green;
        double y3 = ((Pnm_fpr)methods->at(helperArr,col,row+1))->green;
        double y4 = ((Pnm_fpr)methods->at(helperArr,col+1,row+1))->green;

        
        double a = (y4 + y3 + y2 + y1)/4.0;
        double b = (y4 + y3 - y2 - y1)/4.0;
        double c = (y4 - y3 + y2 - y1)/4.0;
        double d = (y4 - y3 -  y2 + y1)/4.0;

        int bcdWidth = 6;
        int prbWidth = 4;
        int aWidth = 6;
        bitword = Bitpack_newu(bitword, prbWidth, 0,  prquant);
        bitword = Bitpack_newu(bitword, prbWidth, prbWidth, 
                                                pbquant);
        bitword = Bitpack_news(bitword, bcdWidth, 2 * prbWidth, 
                                                d);
        bitword = Bitpack_news(bitword, bcdWidth, bcdWidth + 
                                                2 * prbWidth, c);
        bitword = Bitpack_news(bitword, bcdWidth, 
                        2 * bcdWidth + 2 * prbWidth, b);
        bitword = Bitpack_newu(bitword, aWidth, 
                        3 * bcdWidth + 2 * prbWidth, a);
     }
        uint32_t small_word = (uint32_t)(bitword);
        (void) small_word;
        for(int i = 24; i >= 0; i = i - 8){
                unsigned char bytechar = (small_word >> i);
                putchar(bytechar);
        }
    (void) helperArr;
    (void) elem;
    (void) col;
    (void) row;
    (void) array2b;
    (void) methods;
}

/* 
 *  saveinput function, which is used to map on an A2 array and save inputs
 *  from cl to array2b
*/
void saveinput(int col, int row, A2 array2b, void *elem, void *cl)
{
        (void) col;
        (void) row;
        (void) array2b;

        FILE *input = cl;
        assert(input);

        uint64_t *small_word = elem;

        uint64_t bitword = 0;
        uint64_t bytechar;

        for (int i = 24; i >= 0; i -= 8){
                bytechar = getc(input);
                bytechar = bytechar << i;
                bitword = bitword | bytechar;
        }
        
        *small_word = bitword;
}

/* 
 *  unpacking function, which is used to map on an A2 array and unpack its
 *  contents to stdout
*/
void unpacking(int col, int row, A2 array2b, void *elem, void *cl)
{
    A2Methods_T methods = uarray2_methods_plain;
    A2 originalrgb = ((A2*)cl);

    (void) originalrgb;
    (void) elem;
    (void) col;
    (void) row;
    (void) array2b;
    (void) methods;
    (void) cl;

    Pnm_fpr fbr = elem;
    (void) fbr;
 
    uint64_t bitword = *((uint64_t *)elem);

    uint64_t a  = Bitpack_getu(bitword, 6, 
                            3 * 6 + 2 * 4);
    int64_t  b  = Bitpack_gets(bitword, 6, 
                            2 * 6 + 2 * 4);
    int64_t  c  = Bitpack_gets(bitword, 6, 
                            6 + 2 * 4);
    int64_t  d  = Bitpack_gets(bitword, 6, 2 * 4);
    uint64_t pb = Bitpack_getu(bitword, 4, 4);
    uint64_t pr = Bitpack_getu(bitword, 4, 0);

    float avg_pb, avg_pr;

    Pnm_fpr p1 = methods->at(originalrgb, 2 * col, 2 * row);
    Pnm_fpr p2 = methods->at(originalrgb, 2 * col + 1, 2 * row);
    Pnm_fpr p3 = methods->at(originalrgb, 2 * col, 2 * row + 1);
    Pnm_fpr p4 = methods->at(originalrgb, 2 * col + 1, 2 * row + 1);
    avg_pb = Arith40_chroma_of_index(pb);
    avg_pr = Arith40_chroma_of_index(pr);

    float float_a = (a*0.1)/63;
    float float_b = (b*0.1)/103;
    float float_c = (c*0.1)/63;
    float float_d = (d*0.1)/63;

    p1->blue = avg_pb;
    p2->blue = avg_pb;
    p3->blue = avg_pb;
    p4->blue = avg_pb;

    p1->red = avg_pr;
    p2->red = avg_pr;
    p3->red = avg_pr;
    p4->red = avg_pr;

    p1->green = float_a - float_b - float_c + float_d;
    if (p1->green > 1) {
        p1->green = 1.0;
    }
    else if (p1->green < 0) {
        p1->green = 0.0;
    }
    p2->green = float_a - float_b + float_c - float_d;
    if (p2->green > 1) {
        p2->green = 1.0;
    }
    else if (p2->green < 0) {
        p2->green = 0.0;
    }
    p3->green = float_a + float_b - float_c - float_d;
    if (p3->green > 1) {
        p3->green = 1.0;
    }
    else if (p3->green < 0) {
        p3->green = 0.0;
    }
    p4->green = float_a + float_b + float_c + float_d;
    if (p4->green > 1) {
        p4->green = 1.0;
    }
    else if (p4->green < 0) {
        p4->green = 0.0;
    }
}
