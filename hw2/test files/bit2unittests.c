#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <bit2.h>


int main() 
{

    Bit2_T set = Bit2_new(5, 3);
    fprintf(stderr, "Width: %d\n", Bit2_width(set));
    
    fprintf(stderr, "Height: %d\n", Bit2_height(set));
    
    Bit2_put(set, 0, 0, 1);

    fprintf(stderr, "Element at row 0, col 0: %d\n", 
            Bit2_get(set, 0, 0));
    
    Bit2_free(&set);

    return 0;
}