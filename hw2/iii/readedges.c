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
#include <readedges.h>
#include <pnmrdr.h>
#include <assert.h>
#include <stdlib.h>
#include <except.h>

void setIndexToVal(int col, int row, Bit2_T arr, int b, void *pbm);

/*
 * Takes in number of arguments passed to function and the list of arguments
 * as an array of char pointers, stores the bits in the pbm provided in argument
 * 1 into a Bit2_T which is returned.
 * argc must be <= 2, argv[1] must contain a valid pbm with height and width
 * greater than 0.
*/
Bit2_T readEdges(int argc, char *argv[]) {
    
    FILE *infile;

    Pnmrdr_T pbm = checkInput(argc, argv, &infile);
    Bit2_T bitmap = storeInput(pbm);

    Pnmrdr_free(&pbm);
    fclose(infile);

    return bitmap;
}

/*
 * checkInput checks that the input for the program is correct,
 * then proceeds to open the file into *infile. checkInput returns a Pnmrdr_T
 * which contains the contents of the file. argc must be <= 2, argv[1] must 
 * contain a valid pbm with height and width greater than 0.
*/
Pnmrdr_T checkInput(int argc, char *argv[], FILE **infile) {

    Except_T Bad_Input = { "Bad input" };

    /* Checking if number of inputs is correct*/
    assert(argc <= 2);
    
    if (argc == 1) {
        *infile = stdin;
    } else {
        *infile = fopen(argv[1], "r");
    }
    
    Pnmrdr_T pbm = Pnmrdr_new(*infile);

    if (Pnmrdr_data(pbm).type != 1) {
        RAISE(Bad_Input);
        assert(0);
    }
    
    return pbm;
}

/*
 * storeInput creates a new Bit2_T bitmap, stores the values from a pnmrdr_T
 * inside it, and returns it.
 * height and width in pbm must be greater than 0.
*/
Bit2_T storeInput(Pnmrdr_T pbm) {

    int width = Pnmrdr_data(pbm).width;
    int height = Pnmrdr_data(pbm).height;
    assert(width != 0 && height != 0);

    Bit2_T bitmap = Bit2_new(width, height);

    Bit2_map_row_major(bitmap, setIndexToVal, (void *) pbm);
    
    return bitmap;
}

/*
 * setIndexToVal helper function which gets an element at index (row,col)
 * in a pbm holding struct, and stores the same value at the same index
 * in a Bit2_T arr
*/
void setIndexToVal(int col, int row, Bit2_T arr, int b, void *pbm) {

    (void) b;

    int val = Pnmrdr_get(((Pnmrdr_T) pbm));
    Bit2_put(arr, col, row, val);

    return;
}