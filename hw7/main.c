/********************************************************************
* main.c
* Assignment: HW7
* Authors: Theo Dayton (tdayto01),  Sangyu Chen(schen35)
* Date: 12/5/21
********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <except.h>


#include "bitpack.h"
#include "uarray.h"
#include "seq.h"
#include "mem.h"

typedef struct {
  UArray_T *array;
  int usedElems;
  int size;
} Array;

typedef struct UMachine {
    uint32_t registers[8];  /* registers */
    Array memory; /* main memory */
    Seq_T freeMemory; /* free memory used for later use*/
    uint32_t programCounter;
} *UMachine;

typedef uint32_t Um_instruction;
typedef uint32_t Um_value;

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

/*
* Name: initArray
* Purpose: Initializes an existing Array struct
* Input: array struct and initialisize
* Input Specification: None
* Output: Nothing
*/
static inline void initArray(Array *array, int initialSize);

/*
* Name: insertArray
* Purpose: Inserts at element into an existing array struct
* Input: array struct and element of type UArray_T
* Input Specification: None
* Output: Nothing
*/
static inline void insertArray(Array *array, UArray_T element);

/*
* Name: freeArray
* Purpose: Frees an existing array struct
* Input: array struct
* Input Specification: None
* Output: Nothing
*/
static inline void freeArray(Array *array);
/*
* Name: newUM
* Purpose: Creates a new UM and initializes it
* Input: None
* Input Specification: None
* Output: Returns the new UM
*/
static inline UMachine newUM();

/*
* Name: initializeRegisters
* Purpose: Initializes all registers to 0
* Input: A UM
* Input Specification: None
* Output: None, but all registers initialized
*/
static inline void initializeRegisters(UMachine um);

/*
* Name: getRegVal
* Purpose: Gets a register value
* Input: A UM, an index from which to get the register value
* Input Specification: Index should be a value between 0 and 7 inclusive
* Output: Value at that given index of the register returned
*/
static inline Um_value getRegVal(UMachine um, Um_register index);

/*
* Name: setRegVal
* Purpose: Sets a register value
* Input: A UM, an index at which to set the register value, the value to set
* Input Specification: Index should be a value between 0 and 7 inclusive
* Output: Value at that given index of the register set to the provided value
*/
static inline void setRegVal(UMachine um, Um_register index, Um_value val);

/*
* Name: getSegVal
* Purpose: Gets a value within a given segment
* Input: A UM, the segment block, the index within the segment block
* Input Specification: The segment block and the index should be values that
*                      are actually within the memory at this point in time
* Output: Returns value at that given segment at that given index within the
*         segment block
*/
static inline Um_value getSegVal(UMachine um, uint32_t block, uint32_t index);

/*
* Name: getSeg
* Purpose: Gets value of a segment
* Input: A UM, the segment block
* Input Specification: The segment block should be a value that is
*                      actually within the memory at this point in time
* Output: Returns value at that given segment
*/
static inline UArray_T getSeg(UMachine um, uint32_t block);

/*
* Name: setSegVal
* Purpose: Sets a value within a given segment
* Input: A UM, the segment block, the index within the segment block,
*        the value to set
* Input Specification: The segment block and the index should be values that
*                      are actually within the memory at this point in time
* Output: None, but sets value at that given segment at that given index
*         within the segment block
*/
static inline void setSegVal(UMachine um, uint32_t blockIndex, uint32_t index, Um_value val);

/*
* Name: setSeg
* Purpose: Sets value of a segment into a specific block
* Input: A UM, the block index, the segment
* Input Specification: The block index should be a value that is
*                      actually within the memory at this point in time
* Output: None, but sets value of segment into that specific block
*/
static inline void setSeg(UMachine um, uint32_t blockIndex, UArray_T segment);

/*
* Name: freeSeg
* Purpose: Frees a segment of main memory in the UM
* Input: A UM, the index
* Input Specification: The index should be a value that is
*                      actually within the memory at this point in time
* Output: None, but frees segment at the specified index
*/
static inline void freeSeg(UMachine um, uint32_t index);

/*
* Name: freeMainMemory
* Purpose: Frees the main memory of the UM
* Input: A UM
* Input Specification: None
* Output: None, but main memory freed
*/
static inline void freeMainMemory(UMachine um);
static inline void loadInstructions(UMachine um, FILE *input);

/*
* Name: getInstruction
* Purpose: Get the next instruction from the UM
* Input: A UM, a uint32_t program counter
* Input Specification: None
* Output: Returns the next instruction
*/
static inline Um_instruction getInstruction(UMachine um, uint32_t programCounter);

/*
* Name: instructionLength
* Purpose: Get length of instruction
* Input: A file pointer to the input of instructions
* Input Specification: The file pointer should point to a valid file
* Output: Returns length of instruction
*/
static inline unsigned instructionLength(FILE *input);

/*
* Name: getOP
* Purpose: Get OP code of instruction
* Input: A UM instruction
* Input Specification: None
* Output: Returns OP code of instruction
*/
static inline Um_opcode getOP(Um_instruction instruction);

/*
* Name: getRA
* Purpose: Get RA of instruction
* Input: A UM instruction
* Input Specification: The UM instruction must not be a load value instruction
* Output: Returns RA code of instruction
*/
static inline Um_register getRA(Um_instruction instruction);

/*
* Name: getRB
* Purpose: Get RB of instruction
* Input: A UM instruction
* Input Specification: None
* Output: Returns RB code of instruction
*/
static inline Um_register getRB(Um_instruction instruction);

/*
* Name: getRC
* Purpose: Get RC of instruction
* Input: A UM instruction
* Input Specification: None
* Output: Returns RC code of instruction
*/
static inline Um_register getRC(Um_instruction instruction);

/*
* Name: getVal
* Purpose: Get value of instruction
* Input: A UM instruction
* Input Specification: None
* Output: Returns value of instruction
*/
static inline Um_value getVal(Um_instruction instruction);

/*
* Name: getLVRA
* Purpose: Get RA of load value instruction
* Input: A UM instruction
* Input Specification: The UM instruction must be a load value instruction
* Output: Returns RA code of load value instruction
*/
static inline Um_opcode getLVRA(Um_instruction instruction);

/*
* Name: readWord
* Purpose: Reads in the information and packs it into a word
* Input: A file pointer to the input of instructions
* Input Specification: The file pointer should point to a valid file
* Output: Returns the bitpacked word
*/
static inline Um_instruction readWord(FILE *input);

static inline void execute(UMachine um, Um_opcode op_code,
                   Um_register ra,
                   Um_register rb,
                   Um_register rc,
                   Um_register LVra,
                   Um_value val);

/*
* Name: condMove
* Purpose: Runs the conditional move instruction; if value in RC does not
*          equal 0, set value of RB into RA
* Input: A UM, RA, RB, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void condMove(UMachine um, Um_register A, Um_register B, Um_register C);

/*
* Name: segLoad
* Purpose: Runs the segmented load instruction; store the RB segment's
*          RC word into RA
* Input: A UM, RA, RB, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void segLoad(UMachine um, Um_register A, Um_register B, Um_register C);

/*
* Name: segStore
* Purpose: Runs the segmented store instruction; store the RC value into
*          the RA segment's RB word
* Input: A UM, RA, RB, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void segStore(UMachine um, Um_register A, Um_register B, Um_register C);

/*
* Name: addition
* Purpose: Runs the add instruction; add RB and RC values and store in RA
* Input: A UM, RA, RB, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void addition(UMachine um, Um_register A, Um_register B, Um_register C);

/*
* Name: multiplication
* Purpose: Runs the multiplication instruction; multiply RB and RC values
*          and store in RA
* Input: A UM, RA, RB, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void multiplication(UMachine um, Um_register A, Um_register B, Um_register C);

/*
* Name: division
* Purpose: Runs the division instruction; divide RB and RC values
*          and store in RA
* Input: A UM, RA, RB, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void division(UMachine um, Um_register A, Um_register B, Um_register C);

/*
* Name: nand
* Purpose: Runs the nand instruction; nand RB and RC values and store in RA
* Input: A UM, RA, RB, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void nand(UMachine um, Um_register A, Um_register B, Um_register C);

/*
* Name: halt
* Purpose: Runs the halt instruction; stops the program
* Input: A UM
* Input Specification: None
* Output: None, but program stopped
*/
static inline void halt(UMachine um);

/*
* Name: map
* Purpose: Runs the map instruction; maps memory to a block with index RB
*          and length of value of RC
* Input: A UM, RB, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void map(UMachine um, Um_register B, Um_register C);

/*
* Name: ummap
* Purpose: Runs the ummap instruction; ummaps memory from block with index RC
* Input: A UM, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void unmap(UMachine um, Um_register C);

/*
* Name: output
* Purpose: Runs the output instruction; outputs value in RC to stdout
* Input: A UM, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void output(UMachine um, Um_register C);

/*
* Name: input
* Purpose: Runs the input instruction; inputs value from stdin to value of RC
* Input: A UM, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void input(UMachine um, Um_register C);

/*
* Name: loadProg
* Purpose: Runs the load program instruction; loads the program into the UM at
*          segment of RB index and word of RC index
* Input: A UM, RB, RC
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void loadProg(UMachine um, Um_register B, Um_register C);

/*
* Name: loadVal
* Purpose: Runs the load value instruction; loads value into RA
* Input: A UM, RA, value
* Input Specification: None
* Output: None, but instruction executed
*/
static inline void loadVal(UMachine um, Um_register A, uint32_t val);

/*
* Name: startUM
* Purpose: Start the UM by creating it, loading in the instructions,
*          executing the instructions, and freeing it by calling other
*          helper functions
* Input: A file pointer to the input of instructions
* Input Specification: The file pointer should point to a valid file
* Output: None, but the UM is run
*/
static inline void startUM(FILE *input);

/*
* Name: freeUM
* Purpose: Free the UM by freeing various items in the UM (registers/memory)
* Input: A UM
* Input Specification: None
* Output: None, but the UM freed
*/
static inline void freeUM(UMachine um);

/*
* Name: executeUM
* Purpose: Execute the instructions of the UM
* Input: A UM
* Input Specification: None
* Output: None, but the instructions are executed through incrementing
*         the program counter
*/
static inline void executeUM(UMachine um);

/*
* Name: main
* Purpose: Deals with um program's command line arguments
* Input: Command line argument count, command line argument values
* Output: None
*/
int main(int argc, char *argv[])
{
    if (argc <= 2) {
        if (argc == 1) {
            perror("No UM file provided\n");
            exit(EXIT_FAILURE);
        } else { //argc == 2
            FILE *fp = fopen(argv[1], "r");
            startUM(fp);
            fclose(fp);
        }
    } else {
        perror("TOO MANY ARGUMENTS\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

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
            (uint32_t)UArray_length(um->memory.array[0])){

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

/*
* Name: execute
* Purpose: Executes an instruction
* Input: A UM, OP code, RA, RB, RC, Load Value RA, Value
* Output: None, but instruction executed
*/
void execute(UMachine um, Um_opcode op_code,
                   Um_register ra,
                   Um_register rb,
                   Um_register rc,
                   Um_register LVra,
                   Um_value val)
{
    switch(op_code) {
        case CMOV:
            condMove(um, ra, rb, rc);
            break;

        case SLOAD:
            segLoad(um, ra, rb, rc);
            break;

        case SSTORE:
            segStore(um, ra, rb, rc);
            break;

        case ADD:
            addition(um, ra, rb, rc);
            break;

        case MUL:
            multiplication(um, ra, rb, rc);
            break;

        case DIV:
            division(um, ra, rb, rc);
            break;

        case NAND:
            nand(um, ra, rb, rc);
            break;

        case HALT:
            halt(um);
            break;

        case ACTIVATE:
            map(um, rb, rc);
            break;

        case INACTIVATE:
            unmap(um, rc);
            break;

        case OUT:
            output(um, rc);
            break;

        case IN:
            input(um, rc);
            break;

        case LOADP:
            loadProg(um, rb, rc);
            break;

        case LV:
            loadVal(um, LVra, val);
            break;

    }
}

/*
* Name: condMove
* Purpose: Runs the conditional move instruction; if value in RC does not
*          equal 0, set value of RB into RA
* Input: A UM, RA, RB, RC
* Output: None, but instruction executed
*/
void condMove(UMachine um, Um_register A, Um_register B, Um_register C)
{

    if (getRegVal(um, C) != 0) {
        setRegVal(um, A, getRegVal(um, B));
    }
}

/*
* Name: segLoad
* Purpose: Runs the segmented load instruction; store the RB segment's
*          RC word into RA
* Input: A UM, RA, RB, RC
* Output: None, but instruction executed
*/
void segLoad(UMachine um, Um_register A, Um_register B, Um_register C)
{
    Um_value val = getSegVal(um, B, C);
    setRegVal(um, A, val);
}

/*
* Name: segStore
* Purpose: Runs the segmented store instruction; store the RC value into
*          the RA segment's RB word
* Input: A UM, RA, RB, RC
* Output: None, but instruction executed
*/
void segStore(UMachine um, Um_register A, Um_register B, Um_register C)
{
    Um_value val = getRegVal(um, C);
    setSegVal(um, A, B, val);
}

/*
* Name: addition
* Purpose: Runs the add instruction; add RB and RC values and store in RA
* Input: A UM, RA, RB, RC
* Output: None, but instruction executed
*/
void addition(UMachine um, Um_register A, Um_register B, Um_register C)
{
    int valB = getRegVal(um, B);
    int valC = getRegVal(um, C);
    Um_value val = valB + valC;
    setRegVal(um, A, val);
}

/*
* Name: multiplication
* Purpose: Runs the multiplication instruction; multiply RB and RC values
*          and store in RA
* Input: A UM, RA, RB, RC
* Output: None, but instruction executed
*/
void multiplication(UMachine um, Um_register A, Um_register B, Um_register C)
{
    int valB = getRegVal(um, B);
    int valC = getRegVal(um, C);
    Um_value val = valB * valC;
    setRegVal(um, A, val);
}

/*
* Name: division
* Purpose: Runs the division instruction; divide RB and RC values
*          and store in RA
* Input: A UM, RA, RB, RC
* Output: None, but instruction executed
*/
void division(UMachine um, Um_register A, Um_register B, Um_register C)
{
    Um_value valB = getRegVal(um, B);
    Um_value valC = getRegVal(um, C);
    Um_value val = valB / valC;
    setRegVal(um, A, val);
}

/*
* Name: nand
* Purpose: Runs the nand instruction; nand RB and RC values and store in RA
* Input: A UM, RA, RB, RC
* Output: None, but instruction executed
*/
void nand(UMachine um, Um_register A, Um_register B, Um_register C)
{
    Um_value valB = getRegVal(um, B);
    Um_value valC = getRegVal(um, C);
    Um_value val = ~(valB & valC);
    setRegVal(um, A, val);
}

/*
* Name: halt
* Purpose: Runs the halt instruction; stops the program
* Input: A UM
* Output: None, but program stopped
*/
void halt(UMachine um)
{
    um->programCounter = UArray_length(um->memory.array[0]);
}

/*
* Name: map
* Purpose: Runs the map instruction; maps memory to a block with index RB
*          and length of value of RC
* Input: A UM, RB, RC
* Output: None, but instruction executed
*/
void map(UMachine um, Um_register B, Um_register C)
{
    Um_value val_C = getRegVal(um, C);
    Seq_T freeMemory = um->freeMemory;
    uint32_t length = Seq_length(freeMemory);

    UArray_T tempMem = NULL;
    if (length == 0) { /* if there is no previous reserved space */
        TRY
            tempMem = UArray_new(val_C, sizeof(uint32_t));
        EXCEPT(Mem_Failed)
            fprintf(stderr, "Resources Exhausted\n");
            exit(EXIT_FAILURE);
        END_TRY;
        insertArray(&um->memory, tempMem);
        setRegVal(um, B, (int)um->memory.usedElems - 1);
    } else {
        TRY
            tempMem = UArray_new(val_C, sizeof(uint32_t));
        EXCEPT(Mem_Failed)
            fprintf(stderr, "Resources Exhausted\n");
            exit(EXIT_FAILURE);
        END_TRY;
        uint32_t index = (uint32_t)(uintptr_t)Seq_remlo(freeMemory);
        um->memory.array[index] = tempMem;
        setRegVal(um, B, index);
    }
}

/*
* Name: ummap
* Purpose: Runs the ummap instruction; ummaps memory from block with index RC
* Input: A UM, RC
* Output: None, but instruction executed
*/
void unmap(UMachine um, Um_register C)
{
    Um_value index = getRegVal(um, C);
    Seq_T freeMemory = um->freeMemory;

    freeSeg(um, index);
    um->memory.array[index] = NULL;
    Seq_addlo(freeMemory, (void *)(uintptr_t)index);
}

/*
* Name: output
* Purpose: Runs the output instruction; outputs value in RC to stdout
* Input: A UM, RC
* Output: None, but instruction executed
*/
void output(UMachine um, Um_register C)
{
    Um_value val = getRegVal(um, C);

    char valc = (char)val;
    putchar(valc);
}

/*
* Name: input
* Purpose: Runs the input instruction; inputs value from stdin to value of RC
* Input: A UM, RC
* Output: None, but instruction executed
*/
void input(UMachine um, Um_register C)
{
    int input = getchar();
    Um_value val;

	if (input == EOF){
        val = ~0;
    } else {
        val = (uint32_t)input;
    }
    setRegVal(um, C, val);
}

/*
* Name: loadProg
* Purpose: Runs the load program instruction; loads the program into the UM at
*          segment of RB index and word of RC index
* Input: A UM, RB, RC
* Output: None, but instruction executed
*/
void loadProg(UMachine um, Um_register B, Um_register C)
{
    Um_value index = getRegVal(um, B);

    if (index != 0){
        UArray_T newInstructions = getSeg(um, B);
        uint32_t length = UArray_length(newInstructions);
        UArray_T copy = UArray_copy(newInstructions, length);

        setSeg(um, 0, copy);
    }

    uint32_t programCounter = getRegVal(um, C);
    um->programCounter = programCounter;
}

/*
* Name: loadVal
* Purpose: Runs the load value instruction; loads value into RA
* Input: A UM, RA, value
* Output: None, but instruction executed
*/
void loadVal(UMachine um, Um_register A, Um_value val)
{
    setRegVal(um, A, val);
}


void loadInstructions(UMachine um, FILE *input)
{
    uint32_t length = instructionLength(input);

    UArray_T instructions = NULL;
    instructions = UArray_new(length, sizeof(uint32_t));

    for (uint32_t i = 0; i < length; i ++){
        Um_instruction word = readWord(input);
        *(Um_instruction *)UArray_at(instructions, i) = word;
    }
    insertArray(&um->memory,instructions);
}

/*
* Name: getInstruction
* Purpose: Get the next instruction from the UM
* Input: A UM, a uint32_t program counter
* Output: Returns the next instruction
*/
Um_instruction getInstruction(UMachine um, uint32_t programCounter)
{
    UArray_T instructions = um->memory.array[0];
    Um_instruction nextInstruction = *(Um_instruction *)UArray_at(instructions,
                                                        programCounter);

    return nextInstruction;
}

/*
* Name: instructionLength
* Purpose: Get length of instruction
* Input: A file pointer to the input of instructions
* Output: Returns length of instruction
*/
unsigned instructionLength(FILE *input)
{
    fseek(input, 0, SEEK_END);
    unsigned fileLength = ftell(input);
    fseek(input, 0, SEEK_SET);
    return (fileLength / 4);
}

/*
* Name: getOP
* Purpose: Get OP code of instruction
* Input: A UM instruction
* Output: Returns OP code of instruction
*/
Um_opcode getOP(Um_instruction instruction)
{
    uint32_t OP = instruction >> 28;
    return OP;
}

/*
* Name: getRA
* Purpose: Get RA of instruction
* Input: A UM instruction
* Output: Returns RA code of instruction
*/
Um_register getRA(Um_instruction instruction)
{
    uint32_t RA = instruction << 23;
    RA = RA >> 29;
    return RA;
}

/*
* Name: getRB
* Purpose: Get RB of instruction
* Input: A UM instruction
* Output: Returns RB code of instruction
*/
Um_register getRB(Um_instruction instruction)
{
    uint32_t RB = instruction << 26;
    RB = RB >> 29;
    return RB;
}

/*
* Name: getRC
* Purpose: Get RC of instruction
* Input: A UM instruction
* Output: Returns RC code of instruction
*/
Um_register getRC(Um_instruction instruction)
{
    uint32_t RC = instruction << 29;
    RC = RC >> 29;
    return RC;
}

/*
* Name: getVal
* Purpose: Get value of instruction
* Input: A UM instruction
* Output: Returns value of instruction
*/
Um_value getVal(Um_instruction instruction)
{
    uint32_t val = instruction << 7;
    val = val >> 7;
    return val;
}

/*
* Name: getLVRA
* Purpose: Get RA of load value instruction
* Input: A UM instruction
* Output: Returns RA code of load value instruction
*/
Um_opcode getLVRA(Um_instruction instruction)
{
    uint32_t RA = instruction << 4;
    RA = RA >> 29;
    return RA;
}

/*
* Name: readWord
* Purpose: Reads in the information and packs it into a word
* Input: A file pointer to the input of instructions
* Output: Returns the bitpacked word
*/
Um_instruction readWord(FILE *input)
{
    uint64_t word = 0;
    for (int i = 3; i >= 0; i--) {
        word = Bitpack_newu(word, 8, i * 8, (uint64_t)getc(input));
    }
    return word;
}


UMachine newUM()
{
    UMachine newMachine;
    NEW(newMachine);
    /*Allocate and check for resource exhaustion by Mem_Failed exception*/
    initArray(&newMachine->memory,100);
    newMachine->freeMemory =  Seq_new(100);

    newMachine->programCounter = 0;
    initializeRegisters(newMachine);
    return newMachine;
}

/*
* Name: initializeRegisters
* Purpose: Initializes all registers to 0
* Input: A UM
* Output: None, but all registers initialized
*/
void initializeRegisters(UMachine um)
{
    for(int i = 0; i < 8; i ++){
        um->registers[i] = 0;
    }
}

/*
* Name: freeMainMemory
* Purpose: Frees the main memory of the UM
* Input: A UM
* Output: None, but main memory freed
*/
void freeMainMemory(UMachine um)
{
    freeArray(&um->memory);
}

/*
* Name: getRegVal
* Purpose: Gets a register value
* Input: A UM, an index from which to get the register value
* Output: Value at that given index of the register returned
*/
Um_value getRegVal(UMachine um, Um_register index)
{
    return um->registers[index];
}

/*
* Name: setRegVal
* Purpose: Sets a register value
* Input: A UM, an index at which to set the register value, the value to set
* Output: Value at that given index of the register set to the provided value
*/
void setRegVal(UMachine um, Um_register index, Um_value val)
{
    um->registers[index] = val;
}

/*
* Name: getSegVal
* Purpose: Gets a value within a given segment
* Input: A UM, the segment block, the index within the segment block
* Output: Returns value at that given segment at that given index within the
*         segment block
*/
Um_value getSegVal(UMachine um, uint32_t block, uint32_t index)
{
    Um_value block_val = getRegVal(um, block);
    Um_value index_val = getRegVal(um, index);

    UArray_T segment = um->memory.array[block_val];

    return *(Um_value *)UArray_at(segment, index_val);
}

/*
* Name: getSeg
* Purpose: Gets value of a segment
* Input: A UM, the segment block
* Output: Returns value at that given segment
*/
UArray_T getSeg(UMachine um, uint32_t block)
{
    Um_value block_val = getRegVal(um, block);
    UArray_T segment = um->memory.array[block_val];
    return segment;
}

/*
* Name: setSegVal
* Purpose: Sets a value within a given segment
* Input: A UM, the segment block, the index within the segment block,
*        the value to set
* Output: None, but sets value at that given segment at that given index
*         within the segment block
*/
void setSegVal(UMachine um, uint32_t blockIndex, uint32_t index, Um_value val)
{
    Um_value blockIndex_val = getRegVal(um, blockIndex);
    Um_value index_val = getRegVal(um, index);

    UArray_T segment = um->memory.array[blockIndex_val];

    *(Um_value *)UArray_at(segment, index_val) = val;
}

/*
* Name: setSeg
* Purpose: Sets value of a segment into a specific block
* Input: A UM, the block index, the segment
* Output: None, but sets value of segment into that specific block
*/
void setSeg(UMachine um, uint32_t blockIndex, UArray_T segment)
{
    freeSeg(um, blockIndex);
    um->memory.array[blockIndex] = segment;
}

/*
* Name: freeSeg
* Purpose: Frees a segment of main memory in the UM
* Input: A UM, the index
* Output: None, but frees segment at the specified index
*/
void freeSeg(UMachine um, uint32_t index)
{
    UArray_T segment = um->memory.array[index];
    if (segment != NULL){
        UArray_free(&segment);
    }
}

/*
* Name: initArray
* Purpose: Initializes an existing Array struct
* Input: array struct and initialisize
* Input Specification: None
* Output: Nothing
*/
void initArray(Array *array, int size) {
  array->array = malloc(size * sizeof(UArray_T));
  array->usedElems = 0;
  array->size = size;
}

/*
* Name: insertArray
* Purpose: Inserts at element into an existing array struct
* Input: array struct and element of type UArray_T
* Input Specification: None
* Output: Nothing
*/
void insertArray(Array *array, UArray_T uarray) {
  if (array->usedElems == array->size) {
    array->size *= 2;
    array->array = realloc(array->array, array->size * sizeof(UArray_T));
  }
  array->array[array->usedElems++] = uarray;
}

/*
* Name: freeArray
* Purpose: Frees an existing array struct
* Input: array struct
* Input Specification: None
* Output: Nothing
*/
void freeArray(Array *array) {
    for (int i = 0; i < array->usedElems; i++) {
        if (array->array[i] != NULL) {
            UArray_free(&array->array[i]);
    }
    }
  free(array->array);
  array->array = NULL;
  array->usedElems = array->size = 0;
}
