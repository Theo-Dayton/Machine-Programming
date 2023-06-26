#include "uarray2b.h"
#include <stdio.h>

void apply(int col, int row, UArray2b_T array2b, void *elem, void *cl)
{
  (void) col;
  if (row == 5) {
    int x = 5;
    int *point = elem;
    *point = x;
  }

  (void) array2b;
  (void) elem;
  (void) cl;
}

int main(void){

  UArray2b_T stupid = UArray2b_new(15, 15, sizeof(int),4);


  int x = 5;
  int *point = (int *)UArray2b_at(stupid,5,5);
  *point = x; 

  for(int i = 0; i < UArray2b_width(stupid); i++){
    for(int j = 0; j < UArray2b_height(stupid); j++){//
      printf("%d ",*(int *)UArray2b_at(stupid,i,j));
    }
    printf("\n");
  }

  UArray2b_free(&stupid);
}
