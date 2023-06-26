#include "arithmetic.h"

/*
arithmetic.c

Purpose: Imeplement arithmetic operators, which includes addition,
multiplication, division, load_value and output for um

Written by Theo Dayton (tdayto01) and Sangyu Chen (schen35) 

*/

/*
    Function Name: conditional_move
    Description: implement the conditional move operator for um, 
    If RC != 0, then stores the value in RB in register RA.
    Input: machine_state machine, uint32_t wordintint
    Output: void
*/
void conditional_move(machine_state machine, uint32_t wordint)
{
    assert(machine);
    uint32_t RA = getRA(wordint);
    uint32_t RB = getRB(wordint);
    uint32_t RC = getRC(wordint);
    
    if(machine->registers[RC]!=0) 
    {
        machine->registers[RA] = machine->registers[RB];
    }
}


/*
    Function Name: add
    Description: implement the addition operator for um, 
    Adds values stored in RB and RC, mod 2^32, and stores the result in RA.
    Input: machine_state machine, uint32_t wordint
    Output: void
*/
void add(machine_state machine, uint32_t wordint)
{
    assert(machine);
    uint32_t RA = getRA(wordint);
    uint32_t RB = getRB(wordint);
    uint32_t RC = getRC(wordint);
    uint64_t shift = 2;
    shift = shift << 32;
    machine->registers[RA] = ((machine->registers[RB] + 
                                    machine->registers[RC])) % shift;
}


/*
    Function Name: multiply
    Description: implement the multiplication operator for um, 
    Multiplies values stored in RB and RC, mod 2^32, and stores the result 
    in RA.
    Input: machine_state machine, uint32_t wordint
    Output: void
*/
void multiply(machine_state machine, uint32_t wordint) 
{
    assert(machine);
    uint32_t RA = getRA(wordint);
    uint32_t RB = getRB(wordint);
    uint32_t RC = getRC(wordint);
    uint64_t shift = 2;
    shift = shift << 32;
    machine->registers[RA] = ((machine->registers[RB] * 
                                    machine->registers[RC])) % shift;
}

/*
    Function Name: division
    Description: implement the division operator for um, 
    Divides value in RB by the value in RC and stores the result in RA.
    Input: machine_state machine, uint32_t wordint
    Output: void
*/
void division(machine_state machine, uint32_t wordint)
{
    assert(machine);
    uint32_t RA = getRA(wordint);
    uint32_t RB = getRB(wordint);
    uint32_t RC = getRC(wordint);
    if (machine->registers[RC] == 0) {
        fprintf(stderr, "Floating exception\n");
        exit(1);
    }
    machine->registers[RA] = floor((machine->registers[RB] / 
                                    machine->registers[RC]));

}

/*
    Function Name: BitNand
    Description: implement the bitwise nand operator for um, 
    Bitwise AND the value in RB and RC, and negates it, 
    store the result in RA
    Input: machine_state machine, uint32_t wordint
    Output: void
*/
void BitNand(machine_state machine, uint32_t wordint)
{
    assert(machine);
    uint32_t RA = getRA(wordint);
    uint32_t RB = getRB(wordint);
    uint32_t RC = getRC(wordint);
    uint32_t val_B = machine->registers[RB];
    uint32_t val_C = machine->registers[RC];
    uint32_t value5 = ~(val_B & val_C);
    machine->registers[RA] = value5;
}

/*
    Function Name: halt
    Description: implement the halt operator for um, 
    computation stops
    Input: machine_state machine, uint32_t wordint
    Output: void
*/
void halt(machine_state machine, uint32_t wordint)
{
    assert(machine);
    (void) wordint;
    free_machine(machine);
}

/*
    Function Name: load_value
    Description: implement the load value operator for um, 
    Load value to resgister.
    Input: machine_state machine, uint32_t wordint
    Output: void
*/
void load_value(machine_state machine, uint32_t wordint)
{
    assert(machine);
    uint32_t RA = getRA13(wordint);
    uint32_t RB = getRB13(wordint);
    machine->registers[RA] = RB;
}

/*
    Function Name: output
    Description: implement the output operator for um, 
    output the value in the register as character.
    Input: machine_state machine, uint32_t wordint
    Output: void
*/
void output(machine_state machine, uint32_t wordint)
{
    assert(machine);
    uint32_t RC = getRC(wordint);
    assert((machine->registers[RC]) <= 255);
    printf("%c", machine->registers[RC]);
}

/*
    Function Name: input
    Description: implement the input operator for um, 
    RC is loaded with the input, if end of input is signaled,
    RC is loaded with 32 bits with all 1s.
    Input: machine_state machine, uint32_t wordint
    Output: void
*/
void input(machine_state machine, uint32_t wordint)
{
    assert(machine);
    uint32_t RC = getRC(wordint);
    uint32_t value = getc(stdin);
    if ((int)value == -1) {
        machine->registers[RC] = ~(uint32_t)0;
    }
    else {
        if (value > 255) {
            assert(0);
        }
        machine->registers[RC] = value;
    }
}