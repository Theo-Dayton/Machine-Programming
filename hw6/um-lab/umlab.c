/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *  
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.  
 * 
 * Any additional functions and unit tests written for the lab go
 * here. 
 *  
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
    uint64_t instruction = 0;
    instruction = Bitpack_newu(instruction, 4, 28, op);
    instruction = Bitpack_newu(instruction, 3, 6, ra);
    instruction = Bitpack_newu(instruction, 3, 3, rb);
    instruction = Bitpack_newu(instruction, 3, 0, rc);

    return (uint32_t)instruction;
}
Um_instruction loadval(unsigned ra, unsigned val)
{
    uint64_t instruction = 0;
    instruction = Bitpack_newu(instruction, 4, 28, 13);
    instruction = Bitpack_newu(instruction, 3, 25, ra);
    instruction = Bitpack_newu(instruction, 25, 0, val);

    return (uint32_t)instruction;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction conditional_move(Um_register a, Um_register b, Um_register c) 
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction multiply(Um_register a, Um_register b, Um_register c) 
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction division(Um_register a, Um_register b, Um_register c) 
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction bitnand(Um_register a, Um_register b, Um_register c) 
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction segmented_store(Um_register a, Um_register b, Um_register c) 
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction segmented_load(Um_register a, Um_register b, Um_register c) 
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction map_segment(Um_register b, Um_register c) 
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap_segment(Um_register c) 
{
        return three_register(INACTIVATE, 0, 0, c);
}
/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}


/* Unit tests for the UM */

void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}

void build_add_test(Seq_T stream)
{

    append(stream, add(r1, r2, r3));
    append(stream, halt());
}


void build_add2_test(Seq_T stream)
{
    append(stream, add(r2, r3, r1));
    append(stream, halt());
}

void build_print_six_test(Seq_T stream)
{

    append(stream, loadval(r1, 48));
    append(stream, loadval(r2, 6));
    append(stream, add(r3, r1, r2));
    append(stream, output(r3));
    append(stream, halt());
}

void build_multiply_test(Seq_T stream)
{
    append(stream, loadval(r1, 9));
    append(stream, loadval(r2, 6));
    append(stream, multiply(r3, r1, r2));
    append(stream, output(r3));
    append(stream, halt());

}

void build_output_test(Seq_T stream)
{
    append(stream, loadval(r1, 97));
    append(stream, output(r1));
    append(stream, halt());

}



void build_division_test(Seq_T stream)
{
    append(stream, loadval(r1, 122));
    append(stream, loadval(r2, 3));
    append(stream, division(r3, r1, r2));
    append(stream, output(r3));
    append(stream, halt());

}

void build_nand_test(Seq_T stream)
{
   append(stream, loadval(r1, 0xffff));
   append(stream, loadval(r2, 0x10001));
   append(stream, loadval(r3, 0xfff9));
   append(stream, loadval(r4, 0x10007));
   append(stream, multiply(r5, r1, r2));
   append(stream, multiply(r6, r3, r4));
   append(stream, bitnand(r7, r5, r6));
   append(stream, output(r7));
   append(stream, halt());
 
}

// void build_nand2_test(Seq_T stream)
// {
//    append(stream, loadval(r1, 0xffff));
//    append(stream, loadval(r2, 0x10001));
//    append(stream, loadval(r3, 0xfff8));
//    append(stream, loadval(r4, 0x10008));
//    append(stream, multiply(r5, r1, r2));
//    append(stream, multiply(r6, r3, r4));
//    append(stream, bitnand(r7, r5, r6));
//    append(stream, output(r7));
//    append(stream, halt());
 
// }



void build_conditional_test(Seq_T stream)
{
    append(stream, loadval(r1, 97));
    append(stream, loadval(r2, 1));
    append(stream, loadval(r3, 98));
    append(stream, conditional_move(r3, r1, r2));
    append(stream, output(r3));
    append(stream, loadval(r1, 97));
    append(stream, loadval(r2, 0));
    append(stream, loadval(r3, 98));
    append(stream, conditional_move(r3, r1, r2));
    append(stream, output(r3));
    append(stream, halt());


}

void build_divide0_test(Seq_T stream)
{
    append(stream, loadval(r1, 122));
    append(stream, loadval(r2, 0));
    append(stream, division(r3, r1, r2));
    append(stream, output(r3));
    append(stream, halt());

}



void build_map_segment_test(Seq_T stream)
{
    append(stream, loadval(r1, 1));
    append(stream, loadval(r2, 5));
    append(stream, loadval(r3, 10));
    append(stream, loadval(r4, 60));
    append(stream, map_segment(r1, r3));
    append(stream, segmented_store(r1, r2, r4));
    append(stream, segmented_load(r3, r1, r2));
    append(stream, output(r3));
    append(stream, halt());

}


void build_input_test(Seq_T stream)
{
    append(stream, input(r2));
    append(stream, output(r2));
    append(stream, halt());
}


void build_map2_segment_test(Seq_T stream)
{
    append(stream, loadval(r1, 1));
    append(stream, loadval(r2, 5));
    append(stream, loadval(r3, 5));
    append(stream, loadval(r4, 60));
    append(stream, map_segment(r1, r3));
    append(stream, map_segment(r1, r3));
    append(stream, segmented_store(r1, r2, r4));
    append(stream, segmented_load(r3, r1, r2));
    append(stream, output(r3));
    append(stream, halt());
}


void build_map_larger_test(Seq_T stream)
{
    append(stream, loadval(r1, 1));
    append(stream, loadval(r2, 1));
    append(stream, loadval(r3, 3));
    append(stream, loadval(r4, 5));
    append(stream, loadval(r5, 97));
    append(stream, map_segment(r1, r3));
    append(stream, unmap_segment(r1));
    append(stream, map_segment(r1, r4));
    append(stream, segmented_store(r1, r2, r5));
    append(stream, segmented_load(r3, r1, r2));
    append(stream, output(r3));
    append(stream, halt());
}