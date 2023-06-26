#ifndef CHECKSUDOKU_H_INCLUDED
#define CHECKSUDOKU_H_INCLUDED

/*
 *     checksudoku.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains implementations for the checkSudoku function, whose purpose is
 *     to check the validity of a sudoku passed in as a UArray2_T.
 */


#include <uarray2.h>
#include <stdlib.h>
#include <stdio.h>

/* 
 * checkSudoku function which checks the sudoku stored in a UArray2_T is a valid
 * one (i.e. there are no repetitions in rows, columns or 3x3 blocks).
*/
void checkSudoku(UArray2_T sudoku);

#undef CHECKSUDOKU_H_INCLUDED
#endif