
/*
um.c

Purpose: Main driver file for um program

Written by Theo Dayton (tdayto01) and Sangyu Chen (schen35) 

*/

#include "read_file.h"
#include "um.h"
#include "arithmetic.h"
#include "memory.h"
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[])
{
    contract_violation(argc);//first check for contract violations

    Seq_T bitwords = read_file(argv);
    machine_state machine;
    NEW(machine);
    initialize_machine(machine);
    Seq_addhi(machine->program, bitwords);
    Seq_T segment = Seq_get(machine->program, 0);
    while (machine->program_counter < (uint32_t)Seq_length(segment)) {
        segment = Seq_get(machine->program, 0);
        uint32_t word = *(uint32_t *)Seq_get(segment, machine->program_counter);
        int opcode = getopcode(word);
        switch (opcode) {
            case 0:
                conditional_move(machine, word);
                break;

            case 1:
                segmented_load(machine, word);
                break;

            case 2:
                segmented_store(machine, word);
                break;

            case 3:
                add(machine, word);
                break;

            case 4:
                multiply(machine, word);
                break;

            case 5:
                division(machine, word);
                break;

            case 6:
                BitNand(machine, word);
                break;

            case 7:
               halt(machine, word);
               return 0;
               break;

            case 8:
                map_segment(machine, word);
                break;

            case 9:
                unmap_segment(machine, word);
                break;

            case 10:
                output(machine, word);
                break;

            case 11:
                input(machine, word);
                break;

            case 12:
                load_program(machine, word);
                break;

            case 13:
                load_value(machine, word);
                break;

        }
        if (opcode != 12) {
            machine->program_counter++;
        }
    }
    free_machine(machine);
}

/*
    Function Name: contract_violation
    Description: Deal with the numbers of argument, if argc is not equal to 2,
                 exit the program and prints a message to stderr.
    Input: int argc
    Output: none
*/
void contract_violation(int argc)
{
    if (argc == 2) {
        return;
    }
    else {
        fprintf(stderr, "Contract violation: Incorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }
}

/*
    Function Name: initialize_machine
    Description: Initialize registers, program counter, program length and 
                unmapped_segments for the machine.
    Input: machine_state machine
    Output: none
*/
void initialize_machine(machine_state machine)
{
    assert(machine);
    for (int i = 0; i < 8; i++) {
        machine->registers[i] = 0;
    }
    machine->program_counter = 0;
    machine->unmapped_segments = Seq_new(0);
    machine->program = Seq_new(0);
}


/*
    Function Name: free_machine
    Description: free the memory allocated for struct machine_state machine
    Input: machine_state machine
    Output: none
*/
void free_machine(machine_state machine)
{
    assert(machine);
    
    Seq_T program0 = (Seq_get(machine->program,0));
    for (int i = 0; i < Seq_length(program0); i++) {
        uint32_t *finalword = Seq_get(program0, i);
        free(finalword);
    }
    int programlength = Seq_length(machine->program);
    for (int i = 1; i < programlength; i++) {
        if (Seq_length(machine->program) == 0) {
        }
        else {
            UArray_T segment = Seq_get(machine->program, i);
            UArray_free(&segment);
        }
    }
    Seq_free(&(machine->unmapped_segments));
    Seq_free(&program0);
    Seq_free(&(machine->program));
    free(machine);
}

