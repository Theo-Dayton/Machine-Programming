/********************************************************************
* um.c
* Assignment: UM (HW6)
* Authors: Yuehan (Eunice) Dai (ydai05), Grace Ye (gye01)
* Date: 11/23/21
* Summary: This is the implementation of um, which handles
*          the starting, freeing, and execution of the um
********************************************************************/

#include "um.h"
/*
* Name: startUM
* Purpose: Start the UM by creating it, loading in the instructions,
*          executing the instructions, and freeing it by calling other
*          helper functions
* Input: A file pointer to the input of instructions
* Output: None, but the UM is run
*/
void startUM(FILE *input)
{
    UMachine um = newUM();
    loadInstructions(um, input);
    executeUM(um);
    freeUM(um);
}

/*
* Name: freeUM
* Purpose: Free the UM by freeing various items in the UM (registers/memory)
* Input: A UM
* Output: None, but the UM freed
*/
void freeUM(UMachine um)
{
    UArray_free(&(um->registers));
    freeMainMemory(um);
    Seq_free(&(um->freeMemory));
    free(um);
}

/*
* Name: executeUM
* Purpose: Execute the instructions of the UM
* Input: A UM
* Output: None, but the instructions are executed through incrementing
*         the program counter
*/
void executeUM(UMachine um)
{
    int i = 0;
    while (um->programCounter < 
            (uint32_t)UArray_length(Seq_get(um->memory, 0))){

        Um_instruction instruction = getInstruction(um, um->programCounter);
        Um_opcode op_code = getOP(instruction);
        Um_register LVra = getLVRA(instruction);
        Um_value val = getVal(instruction);
        Um_register ra = getRA(instruction);
        Um_register rb = getRB(instruction);
        Um_register rc = getRC(instruction);
        um->programCounter++;
        execute(um, op_code, ra, rb, rc, LVra, val);

        // printR(um);

        i++;
    }
}

void printR(UMachine um)
{
    fprintf(stderr, "%d %d %d %d %d %d %d %d\n", 
                    getRegVal(um, r0), 
                    getRegVal(um, r1), 
                    getRegVal(um, r2),
                    getRegVal(um, r3),
                    getRegVal(um, r4),
                    getRegVal(um, r5),
                    getRegVal(um, r6),
                    getRegVal(um, r7));
}