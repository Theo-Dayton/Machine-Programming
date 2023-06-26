/*
read_file.c

Purpose: Reading in file for our um program

Written by Theo Dayton (tdayto01) and Sangyu Chen (schen35) 

*/
#include "read_file.h"
#include <unistd.h> 

/*
    Function Name: read_file
    Description: Reads in .um file and stores bitwords in a sequence
    Input: char* argv[]
    Output: Seq_T
*/
Seq_T read_file(char* argv[])
{
    FILE *infile;
    infile = fopen(argv[1], "r");
    assert(infile);
    Seq_T bitword = Seq_new(100);
    if (infile == NULL) {
        fprintf(stderr, 
                "%s: %s %s %s \n",
                argv[0], "Could not open file ",
                argv[1], "for reading");
                exit(1);
    }
    else {
        readWord(infile, bitword);
    }
    fclose(infile);
    return bitword;
}

/*
    Function Name: readWord
    Description: Takes in a file stream of bitpacked instructions,
    saves the instruction information to a wordstruct struct,
    then saves the wordstructs to a sequence.
    Input: FILE* input and Sequence bitword
    Output: nothing
*/
void readWord(FILE *input, Seq_T bitword)
{
    assert(input != NULL);
    uint32_t byte = (uint32_t)getc(input);
    uint32_t first8, second8, third8, last8;
    uint32_t *finalword;
    while ((int)byte != EOF) {
        finalword = malloc(sizeof(uint32_t));
        first8 = byte;
        first8 = first8 << 24;
        second8 = (uint32_t)getc(input);
        second8 = second8 << 16;
        third8 = (uint32_t)getc(input);
        third8 = third8 << 8;
        last8 = (uint32_t)getc(input);
        *finalword = first8 | second8 | third8 | last8;
        byte = (uint32_t)getc(input);
        
        Seq_addhi(bitword, (void *)finalword);
    }
}



/*
    Function Name: struct_to_uint
    Description: Takes an instruction and convert it into a uint32_t word
    Input: instruction word
    Output: unint32_t finalword
*/
uint32_t struct_to_uint(instruction word)
{
    uint32_t opcode = word->opcode;
    uint32_t RA = word->RA;
    uint32_t RB = word->RB;
    uint32_t RC = word->RC;
    opcode = opcode << 28;
    RA = RA << 6;
    RB = RB << 3;
    uint32_t finalword = opcode | RA | RB | RC;
    return finalword;
}

/*
    Function Name: uint_to_struct
    Description: Takes a uint32_t word and convert it into an instruction 
    Input: uint32_t word
    Output: instruction wordstruct
*/
instruction uint_to_struct(uint32_t word)
{
    instruction wordstruct = initstruct();
    uint32_t opcode = word >> 28;
    uint32_t RA = word << 23;
    RA = RA >> 29;
    uint32_t RB = word << 26;
    RB = RB >> 29;
    uint32_t RC = word << 29;
    RC = RC >> 29;
    wordstruct->opcode = opcode;
    wordstruct->RA = RA;
    wordstruct->RB = RB;
    wordstruct->RC = RC;
    return wordstruct;
}


/*
    Function Name: initstruct()
    Description: Initialize the instruction struct
    Input: None
    Output: a instance of instruction, all element are initialized to 0.
*/
instruction initstruct()
{
    instruction word = malloc(sizeof(uint32_t)*4);
    word->opcode = 0;
    word->RA = 0;
    word->RB = 0;
    word->RC = 0;
    return word;
}

/*
    Function Name: getopcode
    Description: get the opcode of uint32_t word
    Input: uint32_t word
    Output: the opcode of word
*/

uint32_t getopcode(uint32_t word)
{
    assert(word);
    uint32_t opcode = word >> 28;
    return opcode; 
}

/*
    Function Name: getRA
    Description: get the RA of uint32_t word
    Input: uint32_t word
    Output: the RA of word
*/

uint32_t getRA(uint32_t word)
{
    assert(word);
    uint32_t RA = word << 23;
    RA = RA >> 29;
    return RA;
}

/*
    Function Name: getRB
    Description: get the RB of uint32_t word
    Input: uint32_t word
    Output: the RB of word
*/
uint32_t getRB(uint32_t word)
{
    assert(word);
    uint32_t RB = word << 26;
    RB = RB >> 29;
    return RB;
}

/*
    Function Name: getRC
    Description: get the RC of uint32_t word
    Input: uint32_t word
    Output: the RC of word
*/
uint32_t getRC(uint32_t word)
{
    assert(word);
    uint32_t RC = word << 29;
    RC = RC >> 29;
    return RC;
}

/*
    Function Name: getRA13
    Description: get the RA13 of uint32_t word with opcode 13
    Input: uint32_t word
    Output: the RA of word
*/

uint32_t getRA13(uint32_t word)
{
    assert(word);
    uint32_t RA = word << 4;
    RA = RA >> 29;
    return RA;
}

/*
    Function Name: getRB13
    Description: get the RA13 of uint32_t word with opcode 13
    Input: uint32_t word
    Output: the RB of word
*/
uint32_t getRB13(uint32_t word)
{
    uint32_t RB = word << 7;
    RB = RB >> 7;
    return RB;
}