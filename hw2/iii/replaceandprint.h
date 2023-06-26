#ifndef REPLACEANDPRINT_H_INCLUDED
#define REPLACEANDPRINT_H_INCLUDED

/*
 *     replaceandprint.h
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains implementations for the replaceandprint function, whose purpose
 *     is to find and replace the black edges of a bitmap, stored in a Bit2_T 
 *     struct. The program then prints the resulting bitmap to stdout.
 */


#include <stdlib.h>
#include <stdio.h>
#include <bit2.h>


/*
 * replaceAndPrint function takes in a bitmap as a parameter, replaces any black
 * edge pixels with white, then prints the edited bitmap to standard output.
*/
void replaceAndPrint(Bit2_T bitmap);

/*
 * replaceBlack function takes in a bitmap as a parameter,finds all 
 * black edges in this bitmap, and replaces them with white.
*/
void replaceBlack(Bit2_T bitmap);

/* 
 * printBitMap function calls a mapping function on a Bit2_T bitmap to print out its
 * contents in a correct plain pbm format
*/
void printBitmap(Bit2_T bitmap);

#undef REPLACEANDPRINT_H_INCLUDED
#endif