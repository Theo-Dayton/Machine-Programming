#ifndef UARRAY2_H_INCLUDED
#define UARRAY2_H_INCLUDED

/*
 *     uarray2.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains implementations for the UArray2_T data structure, whose purpose
 *     is to store void pointers in a 2D unboxed array.
 */


#define T UArray2_T
typedef struct T *T;


/* 
 * UArray2_new returns a new UArray2_T struct, which has a width, height, and
 * element size determined by its parameters.
 * All parameters must be non-negative integers.
*/
extern T UArray2_new(int width, int height, int size);


/* 
 * UArray2_width returns the width of the UArray2_T struct arr. arr must point
 * to a valid UArray2_T struct.
*/
extern int UArray2_width(T arr);

/* 
 * UArray2_width returns the height of the UArray2_T struct arr. arr must point
 * to a valid UArray2_T struct.
*/
extern int UArray2_height(T arr);

/* 
 * UArray2_width returns the element size of the UArray2_T struct arr. arr must
 * point to a valid UArray2_T struct.
*/
extern int UArray2_size(T arr);

/* 
 * UArray2_width returns a pointer to an element in the UArray2_T struct arr. 
 * The index of this element is determined by the parameters width and height.
 * arr must point to a valid UArray2_T struct.
*/
extern void *UArray2_at(T arr, int col, int row);

/*
 * UArray2_map_col_major calls an apply function for each element in the array.
 * Row indices vary more rapidly than column indices. When calling the apply
 * function, 'arr' will be passed into the apply's 'arr' parameter, and the
 * value at (col, row) in 'arr' will be passed into the apply's 'p' parameter.
 * arr must point to a valid UArray2_T struct, and the flag pointer must not
 * be null.
*/
extern void UArray2_map_col_major(T arr, 
    void apply(int col, int row, UArray2_T arr, void *p, void *flag), 
    void *flag);

/*
 * UArray2_map_row_major calls an apply function for each element in the array.
 * Column indices vary more rapidly than row indices. When calling the apply
 * function, 'arr' will be passed into the apply's 'arr' parameter, and the
 * value at (col, row) in 'arr' will be passed into the apply's 'p' parameter.
 * arr must point to a valid UArray2_T struct, and the flag pointer must not
 * be null.
*/
extern void UArray2_map_row_major(T arr, 
    void apply(int col, int row, UArray2_T arr, void *p, void *flag), 
    void *flag);

/*
 * UArray2_free frees the memory that was allocated for the UArray2 being
 * pointed to by the first argument.
 * The first argument must not be null, and the UArray2 it points to must also
 * not be null.
*/
extern void UArray2_free(T *arr);


#undef T
#endif