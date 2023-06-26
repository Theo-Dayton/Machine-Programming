#ifndef MEMORY
#define MEMORY


#include "um.h"
#include "read_file.h"
#include <math.h>
#include <assert.h>
#include <uarray.h>
#include <seq.h>
#include <stdbool.h>

void map_segment(machine_state machine, uint32_t wordint);
void unmap_segment(machine_state machine, uint32_t wordint);
void segmented_load(machine_state machine, uint32_t wordint);
void segmented_store(machine_state machine, uint32_t wordint);
void load_program(machine_state machine, uint32_t wordint);


#endif