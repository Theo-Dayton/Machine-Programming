#ifndef READEDGES_H_INCLUDED
#define READEDGES_H_INCLUDED

/*
 *     readedges.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains implementations for the readEdges function, whose purpose is to
 *     read in a file, ensure it is a valid pbm file, and then store its 
 *     contents in a Bit2_T.
 */


#include <bit2.h>
#include <pnmrdr.h>

/*
 * Takes in number of arguments passed to function and the list of arguments
 * as an array of char pointers, stores the bits in the pbm provided in argument
 * 1 into a Bit2_T which is returned.
 * argc must be <= 2, argv[1] must contain a valid pbm.
*/
Bit2_T readEdges(int argc, char *argv[]);

/*
 * checkInput checks that the input for the program is correct,
 * then proceeds to open the file into *infile. checkInput returns a Pnmrdr_T
 * which contains the contents of the file. argc must be <= 2, argv[1] must 
 * contain a valid pbm with height and width greater than 0.
*/
Pnmrdr_T checkInput(int argc, char *argv[], FILE **infile);

/*
 * storeInput creates a new Bit2_T bitmap, stores the values from a pnmrdr_T
 * inside it, and returns it.
 * height and width in pbm must be greater than 0.
*/
Bit2_T storeInput(Pnmrdr_T pbm);

#undef READEDGES_H_INCLUDED
#endif