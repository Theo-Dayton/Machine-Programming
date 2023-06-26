/********************************************************************
* um.h
* Assignment: UM (HW6)
* Authors: Yuehan (Eunice) Dai (ydai05), Grace Ye (gye01)
* Date: 11/23/21
* Summary: This is the interface of um, which handles
*          the starting, freeing, and execution of the um
********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <except.h>


#include "bitpack.h"
#include "uarray.h"
#include "seq.h"
#include "stack.h"
#include "instructions.h"
#include "execute.h"


#ifndef UM
#define UM

/*
* Name: startUM
* Purpose: Start the UM by creating it, loading in the instructions,
*          executing the instructions, and freeing it by calling other
*          helper functions
* Input: A file pointer to the input of instructions
* Input Specification: The file pointer should point to a valid file
* Output: None, but the UM is run
*/
extern void startUM(FILE *input);

/*
* Name: freeUM
* Purpose: Free the UM by freeing various items in the UM (registers/memory)
* Input: A UM
* Input Specification: None
* Output: None, but the UM freed
*/
extern void freeUM(UMachine um);

/*
* Name: executeUM
* Purpose: Execute the instructions of the UM
* Input: A UM
* Input Specification: None
* Output: None, but the instructions are executed through incrementing
*         the program counter
*/
extern void executeUM(UMachine um);

void printR(UMachine um);

#endif