#ifndef UM
#define UM

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <seq.h>
#include <assert.h>
#include <uarray.h>
#include "read_file.h"


typedef struct machine_state {
    uint32_t registers[8];
    uint32_t program_counter;
    Seq_T unmapped_segments;
    Seq_T program;
} *machine_state;


void contract_violation(int argc);
void initialize_machine(machine_state machine);
void free_machine(machine_state machine);

#endif