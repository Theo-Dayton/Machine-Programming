/*
 *     replaceandprint.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Contains implementations for the replaceandprint function, whose purpose
 *     is to find and replace the black edges of a bitmap, stored in a Bit2_T 
 *     struct. The program then prints the resulting bitmap to stdout.
 */

#include <replaceandprint.h>
#include <seq.h>
#include <stdlib.h>
#include <bit2.h>

void printBitmapHelper(int col, int row, Bit2_T arr, int b, void *p1);

/*
 * replaceAndPrint function takes in a bitmap as a parameter, replaces any black
 * edge pixels with white, then prints the edited bitmap to standard output.
*/
void replaceAndPrint(Bit2_T bitmap)
{
    replaceBlack(bitmap);
    printBitmap(bitmap);
}

/*
 * replaceBlack function takes in a bitmap as a parameter,finds all 
 * black edges in this bitmap, and replaces them with white.
*/
void replaceBlack(Bit2_T bitmap)
{
    Seq_T queue = Seq_new(100);

    /*
     * These two for loops pushes the indices of every black element from the 
     * actual edges into the queue to be handled.
    */   
    for (int i = 0; i < Bit2_width(bitmap); i++) {
        if (Bit2_get(bitmap, i, 0) == 1) {
            int *indices = malloc(2 * sizeof(int));
            indices[0] = i;
            indices[1] = 0;
            Seq_addhi(queue, (void *) indices);
        }
        int bottom = Bit2_height(bitmap) - 1;
        if (Bit2_get(bitmap, i, bottom) == 1) {
            int *indices = malloc(2 * sizeof(int));
            indices[0] = i;
            indices[1] = bottom;
            Seq_addhi(queue, (void *) indices);
        }
    }

    for (int j = 1; j < Bit2_height(bitmap); j++) {
        if (Bit2_get(bitmap, 0, j) == 1) {
            int *indices = malloc(2 * sizeof(int));
            indices[0] = 0;
            indices[1] = j;
            Seq_addhi(queue, (void *) indices);
        }
        int right = Bit2_height(bitmap) - 1;
        if (Bit2_get(bitmap, right, j) == 1) {
            int *indices = malloc(2 * sizeof(int));
            indices[0] = right;
            indices[1] = j;
            Seq_addhi(queue, (void *) indices);
        }
    }

    /* 
     * This while loop pops each element from the queue, pushes any black
     * neighbors that element has to the queue, then switches the color of the
     * element.
    */
    while (Seq_length(queue) != 0) {
        int *indices = (int *) Seq_get(queue, 0);
        if (Bit2_get(bitmap, indices[0], indices[1]) == 1) {
            for (int i = -1; i < 2; i += 2) {
                if (indices[0] + i >= 0 && 
                    indices[0] + i < Bit2_width(bitmap)) {
                    
                    if (Bit2_get(bitmap, indices[0] + i, indices[1]) == 1) {
                        int *newIndices = malloc(2 * sizeof(int));
                        newIndices[0] = indices[0] + i;
                        newIndices[1] = indices[1];
                        Seq_addhi(queue, (void *) newIndices);
                    }
                }

                if (indices[1] + i >= 0 && 
                    indices[1] + i < Bit2_height(bitmap)) {
                    
                    if (Bit2_get(bitmap, indices[0], indices[1] + i) == 1) {
                        int *newIndices = malloc(2 * sizeof(int));
                        newIndices[0] = indices[0];
                        newIndices[1] = indices[1] + i;
                        Seq_addhi(queue, (void *) newIndices);
                    }
                }
            }

            Bit2_put(bitmap, indices[0], indices[1], 0);
            free(indices);
        }
        Seq_remlo(queue);
    }
    
    Seq_free(&queue);
}

/* 
 * printBitMap function calls a mapping function on a Bit2_T bitmap to print out its
 * contents in a correct plain pbm format
*/
void printBitmap(Bit2_T bitmap)
{
    printf("P1\n");
    printf("%d %d\n", Bit2_width(bitmap), Bit2_height(bitmap)); /* test width/height */
    int flag = 0;
    Bit2_map_row_major(bitmap,printBitmapHelper, (void *) &flag);
}

/* 
 * printBitmapHelper helper function prints out the value stored at index (col,row)
 * in a Bit2_T struct.
*/
void printBitmapHelper(int col, int row, Bit2_T arr, int b, void *p1)
{
    (void) p1;
    (void) row;
    printf("%d", b);

    if (col == Bit2_width(arr) - 1) {
        printf("\n");
    }
}