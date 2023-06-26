/*
 *     bit2.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains implementations for the Bit2_T data structure, whose purpose is
 *     to store 2D arrays of bits. 
 */

#include <bit2.h>
#include <bit.h>
#include <stdio.h>
#include <mem.h>
#include <assert.h>

#define T Bit2_T

int indexConvert(T set, int col, int row);

struct T {
    int width, height;
    Bit_T bitSet;
};

/* 
 * Bit2_new returns a new Bit2_T struct, which has a width and height
 * determined by its parameters.
 * All parameters must be non-negative integers.
*/
T Bit2_new(int width, int height)
{
    T newBit2;
    NEW(newBit2);
    assert(width >= 0 && height >= 0);
    newBit2->bitSet = Bit_new(width * height);
    newBit2->width = width;
    newBit2->height = height;
    return newBit2;
}

/* 
 * Bit2_width returns the width of the Bit2_T struct set. set must point
 * to a valid Bit2_T struct.
*/
int Bit2_width(T set)
{
    assert(set);
    return set->width;
}

/* 
 * Bit2_height returns the height of the Bit2_T struct set. set must point
 * to a valid Bit2_T struct.
*/
int Bit2_height(T set)
{
    assert(set);
    return set->height;
}

/* 
 * indexConvert helper function which transforms a row and column in a 2D set
 * into the corresponding 1D vector index
*/
int indexConvert(T set, int col, int row)
{
    assert(set);
    return (set->width * row + col);
}

/* 
 * Bit2_put sets the value of (col, row) in 'set' to be 'bit', and returns the
 * value that was in (col, row) before. 
 * Set must be a valid set, col and row must be nonnegative and smaller than
 * width and height, respectively, of the provided set, and bit must be either
 * 0 or 1.
*/
int Bit2_put(T set, int col, int row, int bit)
{
    assert(set);
    assert(col < set->width && col >= 0 && row < set->height && row >= 0);
    int index = indexConvert(set, col, row);
    return Bit_put(set->bitSet, index, bit);
}

/* 
 * Bit2_get returns a byte, represented as an int, that is stored in
 * the T struct set at index (col,row)
 * Set must be a valid set, col and row must be nonnegative and smaller than
 * width and height, respectively, of the provided set
*/
int Bit2_get(T set, int col, int row)
{
    assert(set);
    assert(col < set->width && col >= 0 && row < set->height && row >= 0);
    int index = indexConvert(set, col, row);
    return Bit_get(set->bitSet, index);
}

/*
 * Bit2_map_col_major calls an apply function for each element in the set.
 * Row indices vary more rapidly than column indices. When calling the apply
 * function, 'set' will be passed into the apply's 'set' parameter, and the
 * value at (col, row) in 'set' will be passed into the apply's 'b' parameter.
 * 'set' must point to a valid Bit2_T struct, and the flag pointer must not
 * be null.
*/
void Bit2_map_col_major(T set, 
    void apply(int col, int row, Bit2_T set, int b, void *p1), void *flag)
{
    assert(flag);
    assert(set);
    for (int i = 0; i < set->width; i++) {
        for (int j = 0; j < set->height; j++) {
            apply(i, j, set, Bit2_get(set, i, j), flag);
        }
    }
}

/*
 * UArray2_map_row_major calls an apply function for each element in the array.
 * Column indices vary more rapidly than row indices. When calling the apply
 * function, 'set' will be passed into the apply's 'set' parameter, and the
 * value at (col, row) in 'set' will be passed into the apply's 'b' parameter.
 * 'set' must point to a valid UArray2_T struct, and the flag pointer must not
 * be null.
*/
void Bit2_map_row_major(T set, 
    void apply(int col, int row, Bit2_T set, int b, void *p1), void *flag)
{
    assert(flag);
    assert(set);
    for (int i = 0; i < set->height; i++) {
        for (int j = 0; j < set->width; j++) {
            apply(j, i, set, Bit2_get(set, j, i), flag);
        }
    }
}

/*
 * Bit2_free frees the memory that was allocated for the Bit2 being
 * pointed to by the first argument.
 * The Bit2 it points to must also
 * not be null.
*/
void Bit2_free(T *set)
{
    assert(set && *set);
    Bit_free(&((*set)->bitSet));
    FREE(*set);
}


#undef T