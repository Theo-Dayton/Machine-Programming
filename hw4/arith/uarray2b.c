#include "uarray2b.h"
#include <assert.h>
#include <stdio.h>
#include <mem.h>
#include <math.h>

#define T UArray2b_T

struct T {
    UArray2_T blockArr;
    int block_size;
    int block_width;
    int block_height;
    int width_buffer;
    int height_buffer;
};

T UArray2b_new (int width, int height, int size, int blocksize)
{
  assert(width >= 0 && height >= 0 && blocksize >= 1);

  int block_width = ((width - 1) + (blocksize-1))  / blocksize;
  int width_buffer = blocksize * block_width - width;
  //(dividend + (divisor / 2)) / divisor

  int block_height = ((height - 1) + (blocksize - 1)) / blocksize;
  int height_buffer = blocksize * block_height - height;

  T new_array;
  NEW(new_array);
  new_array->blockArr = UArray2_new(block_width, block_height,
                                    sizeof(UArray2_T));
  UArray2_T temp;
  for(int i = 0; i < block_width; i++){
    for(int j = 0; j < block_height; j++){
      temp = UArray2_new(blocksize, blocksize, size);
      UArray2_T *temp2 = (UArray2_T *)UArray2_at(new_array->blockArr,i,j);
      *temp2 = temp;
    }
  }
  new_array->block_size = blocksize;
  new_array->block_width = block_width;
  new_array->block_height = block_height;
  new_array->width_buffer = width_buffer;
  new_array->height_buffer = height_buffer;

  printf("Block width: %d \n",new_array->block_width);
  printf("Block height: %d \n",new_array->block_height);
  printf("Block Size: %d \n",new_array->block_size);
  printf("Width buffer: %d \n", new_array->width_buffer);
  printf("Height buffer: %d \n",new_array->height_buffer);

  return new_array;
}

 T UArray2b_new_64K_block(int width, int height, int size)
 {
   
     int blocksize = 0;

     if (size > 65535) {
         blocksize = 1;
     }
     else {
         blocksize = sqrt(65536 / size);
     }
     return UArray2b_new(width, height, size, blocksize);
 }

 void UArray2b_free (T *array2b)
{
     for(int i = 0; i < UArray2_width((*array2b)->blockArr); i++){
         for (int j = 0; j < UArray2_height((*array2b)->blockArr); j++)
         {
             UArray2_T* temp = (UArray2_T *)UArray2_at((*array2b)->blockArr,i,j);
             UArray2_free(temp);
         }
     }
     UArray2_free(&(*array2b)->blockArr);
     FREE(*array2b);
 }

 int UArray2b_width (T array2b)
{
 return (array2b->block_width * array2b->block_size) -
        array2b->width_buffer;
}

 int UArray2b_height (T array2b)
 {
   return (array2b->block_height * array2b->block_size) -
          array2b->height_buffer;
 }

 int UArray2b_size (T array2b)
 {
     return UArray2_size(array2b->blockArr);
 }
 int UArray2b_blocksize(T array2b)
 {
   return array2b->block_size;
 }

 void *UArray2b_at(T array2b, int column, int row)
 {
  int block_column = column / array2b->block_size;
  int col_mod = column % array2b->block_size;

  int block_row = row / array2b->block_size;
  int row_mod = row % array2b->block_size;
  UArray2_T block = *(UArray2_T *)UArray2_at(array2b->blockArr,
                     block_column, block_row);
  return UArray2_at(block,col_mod,row_mod);
 }

 void UArray2b_map(T array2b,
void apply(int col, int row, T array2b,
void *elem, void *cl),
void *cl)
{
    for(int i = 0; i < UArray2_width(array2b->blockArr); i++){
      for(int j = 0; j < UArray2_height(array2b->blockArr); j++){
        map_one_block(array2b,i,j,apply,cl);
      }
    }
}

void map_one_block(UArray2b_T array2b, int row, int col, void apply(int col, int row, T array2b,
void *elem, void *cl), void *cl)
{
    int blocksize = UArray2b_blocksize(array2b);
    for(int i = 0; i < blocksize; i++){
      for(int j = 0; j < blocksize; j++){
          int index_row = row * blocksize + i;
          int index_col = col * blocksize + j;
          if (index_row > UArray2b_width(array2b) - (array2b->width_buffer - 1) ||
              index_col > UArray2b_height(array2b) - (array2b->height_buffer - 1)){
                continue;
              }
          apply(index_col, index_row, array2b,
          UArray2b_at(array2b,index_col,index_row), cl);
      }
    }
}
