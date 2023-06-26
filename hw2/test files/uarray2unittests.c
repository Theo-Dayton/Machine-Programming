#include <uarray2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int main()
{
    UArray2_T arr = UArray2_new(5, 3, sizeof(int));
    fprintf(stderr, "Width: %d\n", UArray2_width(arr));
    
    fprintf(stderr, "Height: %d\n", UArray2_height(arr));
    
    fprintf(stderr, "Size: %d\n", UArray2_size(arr));
    
    int *elem = (int *) UArray2_at(arr, 0, 0);
    *elem = 5;
    fprintf(stderr, "Element at row 0, col 0: %d\n", 
            *((int *) UArray2_at(arr, 0, 0)));
    
    UArray2_free(&arr);

    return 0;
}