#include <stdio.h>
#include <stdlib.h>
#include <uarray2.h>

/*
 *     uarray2b.h
 *     by Theo Dayton and Thomson Brand, 10/15/2021
 *     Locality and the costs of loads and stores
 *
 *     Contains interface for the UArray2b data structure, whose purpose
 *     is to store void pointers in a 2D boxed array.
 */

#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED
#define T UArray2b_T

typedef struct T *T;

/*
* new blocked 2d array
* blocksize = square root of # of cells in block.
* blocksize < 1 is a checked runtime error
*/
extern T UArray2b_new (int width, int height, int size, int blocksize);

/* new blocked 2d array: blocksize as large as possible provided
* block occupies at most 64KB (if possible)
*/
extern T UArray2b_new_64K_block(int width, int height, int size);
extern void UArray2b_free (T *array2b);
extern int UArray2b_width (T array2b);
extern int UArray2b_height (T array2b);
extern int UArray2b_size (T array2b);
extern int UArray2b_blocksize(T array2b);

/* return a pointer to the cell in the given column and row.
* index out of range is a checked run-time error
*/

extern void *UArray2b_at(T array2b, int column, int row);

/* visits every cell in one block before moving to another block */

extern void UArray2b_map(T array2b,
void apply(int col, int row, T array2b,
void *elem, void *cl),
void *cl);

void map_one_block(UArray2b_T array2b,int row, int col,
  void apply(int col, int row, T array2b,
void *elem, void *cl),void *cl);

/*
* it is a checked run-time error to pass a NULL T
* to any function in this interface
*/

#undef T
#endif
