#include "memory.h"

/*
memory.c

Purpose: Imeplement memory operators, which includes map_segment, 
        unmap_segment, segmented load, segmented store, and load program.

Written by Theo Dayton (tdayto01) and Sangyu Chen (schen35) 

*/

/*
    Function Name: map_segment
    Description: implement the map segment operator for um, 
    Creates a new map segment with a number of words equal to RC in $m[RB]
    Each word is initialized to 0, 
    Input: machine_state machine, instruction word
    Output: void
*/
void map_segment(machine_state machine, uint32_t wordint)
{
    uint32_t RB = getRB(wordint);
    uint32_t RC = getRC(wordint);
    UArray_T new_seg = UArray_new(machine->registers[RC], sizeof(uint32_t));
    assert(new_seg);
    for (unsigned int i = 0; i < machine->registers[RC]; i++) { 
        uint32_t uintword = 0;
        *(uint32_t *)UArray_at(new_seg, i) = uintword;
    }
    if (Seq_length(machine->unmapped_segments) != 0) {
        uint32_t index = 
                (uint32_t)(uintptr_t)Seq_remhi(machine->unmapped_segments);
        Seq_put(machine->program, index, (void *)(uintptr_t)new_seg);
        machine->registers[RB] = index;
    }
    else {
        Seq_addhi(machine->program, (void *)new_seg);
        uint32_t index = Seq_length(machine->program) - 1;
        machine->registers[RB] = index;
    }
}


/*
    Function Name: unmap_segment
    Description: implement the unmap segment operator for um, 
    Free segment at $m[RC]
    Input: machine_state machine, instruction word
    Output: void
*/
void unmap_segment(machine_state machine, uint32_t wordint)
{
    uint32_t RC = getRC(wordint);
    if (machine->registers[RC] == 0) {
        assert(0);
    }
    UArray_T segment = Seq_get(machine->program, RC);
    (void) segment;
    UArray_free(&segment);
    Seq_addhi(machine->unmapped_segments, (void *)(uintptr_t)RC);
}


/*
    Function Name: segmented_load
    Description: implement the segmented load operator for um, 
    Loads the value in segment $m[[RB][RC]] into register RA
    Input: machine_state machine, instruction word
    Output: void
*/
void segmented_load(machine_state machine, uint32_t wordint)
{   
    uint32_t RA = getRA(wordint);
    uint32_t RB = getRB(wordint);
    uint32_t RC = getRC(wordint);
    if (machine->registers[RB] >= (uint32_t)Seq_length(machine->program)) {
        assert(0);
    }
    uint32_t value = 0;
    if (machine->registers[RB] != 0) {
        UArray_T segment = (UArray_T)Seq_get(machine->program, 
                                                machine->registers[RB]);
        value = *(uint32_t *)UArray_at(segment, machine->registers[RC]);
    }
    else {
        Seq_T segment = (Seq_T)Seq_get(machine->program, 
                                                machine->registers[RB]);
        if (machine->registers[RC] >= (uint32_t)Seq_length(segment)) {
            assert(0);
        }
        value = *(uint32_t *)Seq_get(segment, machine->registers[RC]);
    }
    machine->registers[RA] = value;
}

/*
    Function Name: segmented_store
    Description: implement the segmented store operator for um, 
    Stores the value in register RC into segment $m[[RA][RB]].
    Input: machine_state machine, instruction word
    Output: void
*/
void segmented_store(machine_state machine, uint32_t wordint)
{
    uint32_t RA = getRA(wordint);
    uint32_t RB = getRB(wordint);
    uint32_t RC = getRC(wordint);
    if (machine->registers[RA] != 0) {
        UArray_T segment = (UArray_T)Seq_get(machine->program, 
                                            machine->registers[RA]);
        *(uint32_t *)UArray_at(segment, machine->registers[RB]) = 
                                                    machine->registers[RC];
    }
    else {
        Seq_T segment = Seq_get(machine->program, machine->registers[RA]);
        *(uint32_t *)Seq_get(segment, machine->registers[RB]) = 
                                                    machine->registers[RC];
    }
    
}

/*
    Function Name: load_program
    Description: implement the load program operator for um, 
    Segment $m[RB] is duplicated, replacing $m[0]. 
    Program counter set to point $m[0][RC].
    Input: machine_state machine, instruction word
    Output: void
*/
void load_program(machine_state machine, uint32_t wordint)
{
    uint32_t RB = getRB(wordint);
    uint32_t RC = getRC(wordint);
    if (machine->registers[RB] == 0) {
        machine->program_counter = machine->registers[RC];
        return;
    }
    Seq_T program0 = (Seq_T)Seq_get(machine->program, 0);
    Seq_free(&program0);
    UArray_T copy_seg = (UArray_T)Seq_get(machine->program, 
                                        machine->registers[RB]);
    Seq_T new_seg = Seq_new(0);
    for (int i = 0; i < UArray_length(copy_seg); i++) {
        uint32_t *new_word = malloc(sizeof(uint32_t));
        *new_word = *(uint32_t *)UArray_at(copy_seg, i);
        Seq_addhi(new_seg, (void *)new_word);
    }
    *(Seq_T *)Seq_get(machine->program, 0) = new_seg;
    machine->program_counter = machine->registers[RC];
}