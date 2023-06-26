/*
 *     sudoku.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Driver file for the sudoku program, whose purpose is to read in a pgm
 *     file, confirm whether it represents a correct sudoku, then return 0 if
 *     the sudoku is solved correctly, or 1 otherwise.
 */

#include <stdlib.h>
#include <uarray2.h>
#include <assert.h>
#include <readsudoku.h>
#include <checksudoku.h>


int main (int argc, char *argv[]) 
{

    UArray2_T sudoku = readSudoku(argc, argv);
    checkSudoku(sudoku);
    UArray2_free(&sudoku);

    return 0;
}