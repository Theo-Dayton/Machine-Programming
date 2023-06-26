#ifndef READ_FILE
#define READ_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mem.h>
#include <seq.h>
#include <assert.h>
#include <bitpack.h>

/* If opcode is 13, then RB will hold the value for the value to be loaded 
    into register RA */
typedef struct instruction {
    uint32_t opcode;
    uint32_t RA;
    uint32_t RB;
    uint32_t RC;  
} *instruction;

Seq_T read_file(char* argv[]);
void readWord(FILE *input, Seq_T bitword);
instruction initstruct();
uint32_t getopcode(uint32_t word);
uint32_t getRA(uint32_t word);
uint32_t getRA13(uint32_t word);
uint32_t getRB13(uint32_t word);
uint32_t getRB(uint32_t word);
uint32_t getRC(uint32_t word);
uint32_t struct_to_uint(instruction word);
instruction uint_to_struct(uint32_t word);

#endif