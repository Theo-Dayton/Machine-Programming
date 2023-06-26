/*
 *     checksudoku.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains implementations for the checkSudoku function, whose purpose is
 *     to check the validity of a sudoku passed in as a UArray2_T.
 */

#include <checksudoku.h>
#include <uarray2.h>
#include <stdio.h>
#include <uarray.h>

void setMarker(UArray2_T sudoku, int i, int j, int *nums);

/* 
 * checkSudoku function which checks the sudoku stored in a UArray2_T is a valid
 * one (i.e. there are no repetitions in rows, columns or 3x3 blocks).
*/
void checkSudoku(UArray2_T sudoku) {

    /* checks rows */

    for (int i = 0; i < 9; i++) {
        int rows[9] = { 0 };
        for (int j = 0; j < 9; j++) {
            setMarker(sudoku, j, i, rows);
        }
    }

    /* checks columns */

    for (int i = 0; i < 9; i++) {
        int cols[9] = { 0 };
        for (int j = 0; j < 9; j++) {
            setMarker(sudoku, i, j, cols);
        }
    }


    /* checks 3x3 blocks */

    /* this outer loop gives the inner loop the top corner of each box */
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            int nums[9] = { 0 };

            /* this inner loop checks one box */
            for (int k = i; k < i + 3; k++) {
                for (int l = j; l < j + 3; l++) {
                    setMarker(sudoku, k, l, nums);
                }
            }
        }
    }
}

/* 
 * setMarker helper function sets a marker on indices in the array to check
 * for repetitions in rows or columns. It takes in the sudoku as a UArray2_T,
 * the indices in the sudoku being checked, and an int array of size 9.
*/
void setMarker(UArray2_T sudoku, int i, int j, int *nums) {
    int val = *((int *) UArray2_at(sudoku, i, j)) - 1;
    if (nums[val] > 0) {
        UArray2_free(&sudoku);
        exit(1);
    }
    nums[val]++;
}