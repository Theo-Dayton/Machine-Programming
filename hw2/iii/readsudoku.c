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
#include <readsudoku.h>
#include <pnmrdr.h>
#include <assert.h>
#include <stdlib.h>
#include <except.h>

void setIndexToVal(int col, int row, UArray2_T arr, void *p, void *pgm);

/*
 * Takes in number of arguments passed to function and the list of arguments
 * as an array of char pointers, stores the data in the pgm provided in argument
 * 1 into a UArray2_T which is returned.
 * argc must be <= 2, argv[1] must contain a valid pgm.
*/
UArray2_T readSudoku(int argc, char *argv[]) {
    
    FILE *infile;

    Pnmrdr_T pgm = checkInput(argc, argv, &infile);
    UArray2_T sudoku = storeInput(pgm);

    Pnmrdr_free(&pgm);
    fclose(infile);

    return sudoku;
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
    
    Pnmrdr_T pgm = Pnmrdr_new(*infile);

    if (Pnmrdr_data(pgm).type != 2) {
        RAISE(Bad_Input);
        assert(0);
    }
    
    return pgm;
}

/*
 * storeInput creates a new UArray2_T, stores the values from a pnmrdr_T
 * inside it, and returns it.
 * height and width in pgm must be greater than 0.
*/
UArray2_T storeInput(Pnmrdr_T pgm) {

    int width = Pnmrdr_data(pgm).width;
    int height = Pnmrdr_data(pgm).height;
    int maxval = Pnmrdr_data(pgm).denominator;
    if (width != 9 || height != 9 || maxval != 9) {
        exit(1);
    }

    UArray2_T sudoku = UArray2_new(width, height, sizeof(int));

    UArray2_map_row_major(sudoku, setIndexToVal, (void *) pgm);
    
    return sudoku;
}

/*
 * setIndexToVal helper function which gets an element at index (row,col)
 * in a pbm holding struct, and stores the same value at the same index
 * in a Uarray2_T arr
*/
void setIndexToVal(int col, int row, UArray2_T arr, void *p, void *pgm) {

    (void) p;

    int *val = (int *) UArray2_at(arr, col, row);
    *val = Pnmrdr_get(((Pnmrdr_T) pgm));
    if (*val == 0) {
        exit(1);
    }

    return;
}