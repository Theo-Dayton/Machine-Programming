#ifndef BIT2_H_INCLUDED
#define BIT2_H_INCLUDED

/*
 *     bit2.h
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains the interface for the Bit2_T data structure, whose purpose is
 *     to store 2D arrays of bits. 
 */


#define T Bit2_T
typedef struct T *T;

/* 
 * Bit2_new returns a new Bit2_T struct, which has a width and height
 * determined by its parameters.
 * All parameters must be non-negative integers.
*/
extern T Bit2_new(int width, int height);

/* 
 * Bit2_width returns the width of the Bit2_T struct set. set must point
 * to a valid Bit2_T struct.
*/
extern int Bit2_width(T set);

/* 
 * Bit2_height returns the height of the Bit2_T struct set. set must point
 * to a valid Bit2_T struct.
*/
extern int Bit2_height(T set);

/* 
 * Bit2_put sets the value of (col, row) in 'set' to be 'bit', and returns the
 * value that was in (col, row) before. 
 * Set must be a valid set, col and row must be nonnegative and smaller than
 * width and height, respectively, of the provided set, and bit must be either
 * 0 or 1.
*/
extern int Bit2_put(T set, int col, int row, int bit);

/* 
 * Bit2_get returns a byte, represented as an int, that is stored in
 * the T struct set at index (col,row)
 * Set must be a valid set, col and row must be nonnegative and smaller than
 * width and height, respectively, of the provided set
*/
extern int Bit2_get(T set, int col, int row);

/*
 * Bit2_map_col_major calls an apply function for each element in the set.
 * Row indices vary more rapidly than column indices. When calling the apply
 * function, 'set' will be passed into the apply's 'set' parameter, and the
 * value at (col, row) in 'set' will be passed into the apply's 'b' parameter.
 * 'set' must point to a valid Bit2_T struct, and the flag pointer must not
 * be null.
*/
extern void Bit2_map_col_major(T set, 
    void apply(int col, int row, Bit2_T set, int b, void *p1), void *flag);

/*
 * UArray2_map_row_major calls an apply function for each element in the array.
 * Column indices vary more rapidly than row indices. When calling the apply
 * function, 'set' will be passed into the apply's 'set' parameter, and the
 * value at (col, row) in 'set' will be passed into the apply's 'b' parameter.
 * 'set' must point to a valid UArray2_T struct, and the flag pointer must not
 * be null.
*/
extern void Bit2_map_row_major(T set, 
    void apply(int col, int row, Bit2_T set, int b, void *p1), void *flag);

/*
 * Bit2_free frees the memory that was allocated for the Bit2 being
 * pointed to by the first argument.
 * The Bit2 it points to must also
 * not be null.
*/
extern void Bit2_free(T *set);

#undef T
#endif