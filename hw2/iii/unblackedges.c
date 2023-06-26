/*
 *     unblackedges.c
 *     by Aaron Hamburg and Theo Dayton, 10/4/2021
 *     Interfaces, Implementations, and Images
 *
 *     Driver file for the unblackedges program, whose purpose is to read in a
 *     pbm from a file or from standard input, then replace any black edge
 *     pixels with white pixels, then print the new image to standard output.
 */

#include <stdlib.h>
#include <bit2.h>
#include <assert.h>
#include <readedges.h>
#include <replaceandprint.h>

int main(int argc, char *argv[])
{
    Bit2_T bitmap = readEdges(argc, argv);

    replaceAndPrint(bitmap);

    Bit2_free(&bitmap);
    
}