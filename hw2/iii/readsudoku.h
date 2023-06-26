#ifndef READSUDOKU_H_INCLUDED
#define READSUDOKU_H_INCLUDED

/*
 *     readsudoku.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains implementations for the readSudoku function, whose purpose is to
 *     read in a file, ensure it is both a valid pgm file and has the correct
 *     height, width, and maxval to be a sudoku, and then store its contents
 *     in a UArray2_T struct.
 */

#include <uarray2.h>
#include <pnmrdr.h>

/*
 * Takes in number of arguments passed to function and the list of arguments
 * as an array of char pointers, stores the data in the pgm provided in argument
 * 1 into a UArray2_T which is returned.
 * argc must be <= 2, argv[1] must contain a valid pgm.
*/
UArray2_T readSudoku(int argc, char *argv[]);

/*
 * checkInput checks that the input for the program is correct,
 * then proceeds to open the file into *infile. checkInput returns a Pnmrdr_T
 * which contains the contents of the file. argc must be <= 2, argv[1] must 
 * contain a valid pbm with height and width greater than 0.
*/
Pnmrdr_T checkInput(int argc, char *argv[], FILE **infile);

/*
 * storeInput creates a new UArray2_T, stores the values from a pnmrdr_T
 * inside it, and returns it.
 * height and width in pgm must be greater than 0.
*/
UArray2_T storeInput(Pnmrdr_T pgm);


#undef READSUDOKU_H_INCLUDED
#endif