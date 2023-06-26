/*
 *     uarray2.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains implementations for the UArray2_T data structure, whose purpose
 *     is to store void pointers in a 2D unboxed array.
 */

#include <uarray2.h>
#include <uarray.h>
#include <stdio.h>
#include <mem.h>
#include <assert.h>

#define T UArray2_T

int indexConvert(T arr, int col, int row);

struct T {
    int width, height, size;
    UArray_T smallArr;
};


/* 
 * UArray2_new returns a new UArray2_T struct, which has a width, height, and
 * element size determined by its parameters.
 * All parameters must be non-negative integers.
*/
T UArray2_new(int width, int height, int size)
{
    T newArr;
    NEW(newArr);
    assert(width >= 0 && height >= 0 && size > 0);
    newArr->smallArr = UArray_new(width * height, size);
    newArr->width = width;
    newArr->height = height;
    newArr->size = size;
    return newArr;
}


/* 
 * UArray2_width returns the width of the UArray2_T struct arr. arr must point
 * to a valid UArray2_T struct.
*/
int UArray2_width(T arr)
{
    assert(arr);
    return arr->width;
}

/* 
 * UArray2_height returns the height of the UArray2_T struct arr. arr must point
 * to a valid UArray2_T struct.
*/
int UArray2_height(T arr)
{
    assert(arr);
    return arr->height;
}

/* 
 * UArray2_size returns the element size of the UArray2_T struct arr. arr must
 * point to a valid UArray2_T struct.
*/
int UArray2_size(T arr)
{
    assert(arr);
    return arr->size;
}

/* 
 * indexConvert helper function which transforms a row and column in a 2D array
 * into the corresponding 1D array index
*/
int indexConvert(T arr, int col, int row)
{
    assert(arr);
    return (arr->width * row + col);
}

/* 
 * UArray2_width returns a pointer to an element in the UArray2_T struct arr. 
 * The index of this element is determined by the parameters width and height.
 * arr must point to a valid UArray2_T struct.
*/
void *UArray2_at(T arr, int col, int row)
{
    assert(arr);
    assert(col < arr->width && col >= 0 && row < arr->height && row >= 0);
    return UArray_at(arr->smallArr, indexConvert(arr,col,row));
}

/*
 * UArray2_map_col_major calls an apply function for each element in the array.
 * Row indices vary more rapidly than column indices.
 * arr must point to a valid UArray2_T struct, and the flag pointer must not
 * be null.
*/
void UArray2_map_col_major(T arr, 
    void apply(int col, int row, UArray2_T arr, void *p, void *flag), 
    void *flag)
{
    assert(flag);
    assert(arr);
    for (int i = 0; i < arr->width; i++) {
        for (int j = 0; j < arr->height; j++) {
            apply(i, j, arr, UArray2_at(arr, i, j), flag);
        }
    }
}

/*
 * UArray2_map_row_major calls an apply function for each element in the array.
 * Column indices vary more rapidly than row indices. When calling the apply
 * function, 'arr' will be passed into the apply's 'arr' parameter, and the
 * value at (col, row) in 'arr' will be passed into the apply's 'p' parameter.
 * arr must point to a valid UArray2_T struct, and the flag pointer must not
 * be null.
*/
void UArray2_map_row_major(T arr, 
    void apply(int col, int row, UArray2_T arr, void *p, void *flag), 
    void *flag)
{
    assert(flag);
    assert(arr);
    for (int i = 0; i < arr->height; i++) {
        for (int j = 0; j < arr->width; j++) {
            apply(j, i, arr, UArray2_at(arr, j, i), flag);
        }
    }
}

/*
 * UArray2_free frees the memory that was allocated for the UArray2 being
 * pointed to by the first argument. When calling the apply
 * function, 'arr' will be passed into the apply's 'arr' parameter, and the
 * value at (col, row) in 'arr' will be passed into the apply's 'p' parameter.
 * The first argument must not be null, and the UArray2 it points to must also
 * not be null.
*/
void UArray2_free(T *arr)
{
    assert(arr && *arr);
    UArray_free(&((*arr)->smallArr));
    FREE(*arr);
}

#undef T