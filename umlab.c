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

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc) {
        uint64_t word = 0;
        //printf("op %d\n", op);
        word = Bitpack_newu(word, 4, 28, op);
        word = Bitpack_newu(word, 3, 6, ra);
        word = Bitpack_newu(word, 3, 3, rb);
        word = Bitpack_newu(word, 3, 0, rc);
        uint32_t instruction = (uint32_t)word;
       // printf("instruction: %u \n", instruction);
        return instruction;
}

Um_instruction loadval(unsigned ra, unsigned val) {
        uint64_t word = 0;
        word = Bitpack_newu(word, 4, 28, 13);
        word = Bitpack_newu(word, 3, 25, ra);
        word = Bitpack_newu(word, 25, 0, val);
        uint32_t instruction = (uint32_t)word;
        //printf("instructionloadval: %u \n", instruction);
        return instruction;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction conditional_move(Um_register a, Um_register b, 
Um_register c) {
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction segmented_load(Um_register a, Um_register b, 
Um_register c) {
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction segmented_store(Um_register a, Um_register b,
 Um_register c) {
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction multiplication(Um_register a, Um_register b, 
Um_register c) {
        return three_register(MUL, a, b, c);
}

static inline Um_instruction division(Um_register a, Um_register b, 
Um_register c) {
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, 
Um_register c) {
        return three_register(NAND, a, b, c);
}

static inline Um_instruction map_segment(Um_register b, Um_register c) {
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap_segment(Um_register c) {
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction output(Um_register c) {
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction input(Um_register c) {
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction load_program(Um_register b, Um_register c) {
        return three_register(LOADP, 0, b, c);
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

void build_add_test(Seq_T stream)
{
        append(stream, loadval(r2, 10));
        append(stream, loadval(r3, 38));
        append(stream, add(r1, r2, r3));
        append(stream, output(r1));
        append(stream, halt());

}

void build_output_test(Seq_T stream) 
{       
        append(stream, output(r0));
        append(stream, output(r1));
        append(stream, output(r2));
        append(stream, output(r3));
        append(stream, output(r4));
        append(stream, output(r5));
        append(stream, output(r6));
        append(stream, output(r7));
        append(stream, halt());
}

void build_loadval_test(Seq_T stream) 
{       
        append(stream, loadval(r0, 97));
        append(stream, output(r0));
        append(stream, loadval(r1, 98));
        append(stream, output(r1));
        append(stream, loadval(r2, 99));
        append(stream, output(r2));
        append(stream, loadval(r3, 100));
        append(stream, output(r3));
        append(stream, loadval(r4, 101));
        append(stream, output(r4));
        append(stream, loadval(r5, 102));
        append(stream, output(r5));
        append(stream, loadval(r6, 103));
        append(stream, output(r6));
        append(stream, loadval(r7, 104));
        append(stream, output(r7));
        append(stream, halt());
}

void build_input_test(Seq_T stream) 
{       
        append(stream, input(r0));
        append(stream, output(r0));
        append(stream, input(r1));
        append(stream, output(r1));
        append(stream, input(r2));
        append(stream, output(r2));
        append(stream, input(r3));
        append(stream, output(r3));
        append(stream, input(r4));
        append(stream, output(r4));
        append(stream, input(r5));
        append(stream, output(r5));
        append(stream, input(r6));
        append(stream, output(r6));
        append(stream, input(r7));
        append(stream, output(r7));
        append(stream, halt());
}


void build_segmented_load_store_test(Seq_T stream) 
{       
        append(stream, loadval(r4, 49));
        append(stream, map_segment(r2, r4));
        append(stream, loadval(r0, 1));
        append(stream, loadval(r1, 2));
        append(stream, segmented_store(r0, r1, r4));
        append(stream, segmented_load(r6, r0, r1));
        append(stream, output(r6));
        append(stream, halt());
}

void build_segmented_load_store_test2(Seq_T stream) 
{       
        append(stream, loadval(r4, 55));
        append(stream, map_segment(r2, r4));
        append(stream, loadval(r0, 1));
        append(stream, loadval(r1, 2));
        append(stream, segmented_store(r0, r1, r4));
        append(stream, segmented_load(r6, r0, r1));
        append(stream, output(r6));
        append(stream, halt());
}

void build_map_test(Seq_T stream) 
{       
        append(stream, loadval(r3, 3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, output(r2));
        append(stream, halt());
}

void build_unmap_test(Seq_T stream) 
{       
        append(stream, loadval(r3, 3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));
        append(stream, map_segment(r2, r3));   
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, map_segment(r2, r3)); 
        append(stream, loadval(r4, 48));
        append(stream, unmap_segment(r4)); 
        append(stream, map_segment(r2, r3));   
        append(stream, output(r2));
        append(stream, map_segment(r2, r3));
        append(stream, output(r2));
        append(stream, halt());
}

void build_multiply_test(Seq_T stream) 
{       
        append(stream, input(r1));
        append(stream, output(r1));
        append(stream, input(r2));
        append(stream, output(r2));
        append(stream, input(r3));
        append(stream, output(r3));
        append(stream, multiplication(r0, r1, r2));
        append(stream, output(r0));
        append(stream, loadval(r4, 9));
        append(stream, loadval(r5, 8));
        append(stream, multiplication(r6, r4, r5));
        append(stream, output(r6));
        append(stream, halt());
}

void build_division_test(Seq_T stream) 
{       
        append(stream, input(r1));
        append(stream, output(r1));
        append(stream, input(r2));
        append(stream, output(r2));
        append(stream, input(r3));
        append(stream, output(r3));
        append(stream, division(r0, r1, r2));
        append(stream, output(r0));
        append(stream, loadval(r4, 480));
        append(stream, loadval(r5, 10));
        append(stream, division(r6, r4, r5));
        append(stream, output(r6));
        append(stream, halt());
}

void build_nand_test(Seq_T stream)
{       
        append(stream, loadval(r1, 33554431));
        append(stream, loadval(r2, 128));
        append(stream, multiplication(r3, r1, r2)); 
        append(stream, loadval(r4, 33554431));
        append(stream, loadval(r5, 128));
        append(stream, multiplication(r6, r4, r5));
        append(stream, nand(r7, r3, r6));
        append(stream, loadval(r1, 2));
        append(stream, division(r7, r7, r1));
        append(stream, output(r7));
        append(stream, halt());
}

void print_six(Seq_T stream)
{
        append(stream, input(r6));
        // append(stream, input(r6));
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        append(stream, output(r3));
        append(stream, output(r6));
        append(stream, output(r6));
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

void conditional_move_test(Seq_T stream)
{
        append(stream, loadval(r1, 3));
        append(stream, loadval(r2, 5));
        append(stream, add(r1, r1, r2));
        append(stream, output(r1));
        append(stream, output(r2));
        append(stream, multiplication(r1, r1, r2));
        append(stream, conditional_move(r4, r2, r1));
        append(stream, output(r3));
        append(stream, conditional_move(r7, r3, r6));
        append(stream, conditional_move(r4, r2, r1));
        append(stream, conditional_move(r2, r6, r4));
        append(stream, conditional_move(r7, r5, r2));
        
        append(stream, output(r0));
        append(stream, output(r1));
        append(stream, output(r2));
        append(stream, output(r3));
        append(stream, output(r4));
        append(stream, output(r5));
        append(stream, output(r6));
        append(stream, output(r7));

        append(stream, halt());
}

void build_load_program_test(Seq_T stream) 
{       
        append(stream, loadval(r0, 48));        
        append(stream, map_segment(r1, r0));
        append(stream, loadval(r2, 2)); //r2 = 2
        append(stream, loadval(r3, 18)); //r3 = halt
        append(stream, loadval(r4, 17)); // r4 = output
        append(stream, loadval(r5, 16)); // r5 = store from memory
        append(stream, loadval(r6, 0)); // r6 = 0

        append(stream, segmented_store(r1, r6, r0)); // [1][0] = 48

        append(stream, segmented_load(r7, r6, r5)); // r7 = store from memory
        append(stream, segmented_store(r1, r1, r7));   
        
        append(stream, segmented_load(r7, r6, r4)); // r7 = output
        append(stream, segmented_store(r1, r2, r7)); // [1][2] = output

        append(stream, segmented_load(r7, r6, r3)); // r7 = halt
        append(stream, loadval(r2, 3)); //r2 = 3
        append(stream, segmented_store(r1, r2, r7)); // [1][3] = halt

        append(stream, load_program(r1, r6));
        append(stream, segmented_load(r6, r6, r6));
        append(stream, output(r6));
        append(stream, halt());
}