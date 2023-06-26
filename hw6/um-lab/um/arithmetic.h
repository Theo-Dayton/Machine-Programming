#ifndef ARITHMETIC
#define ARITHMETIC

#include "um.h"
#include "read_file.h"
#include <math.h>
#include <assert.h>

void conditional_move(machine_state machine, uint32_t word);
void add(machine_state machine, uint32_t word);
void multiply(machine_state machine, uint32_t word);
void division(machine_state machine, uint32_t word);
void BitNand(machine_state machine, uint32_t word);
void halt(machine_state machine, uint32_t word);
void output(machine_state machine, uint32_t word);
void input(machine_state machine, uint32_t word);
void load_value(machine_state machine, uint32_t word);


#endif