/**************************************************************
 *
 *                     lilum.c
 *
 *     Assignment: um
 *     Authors:  Youssed Ezzo (yezzo01), Kerwin Teh (kteh01)
 *     Date:     11/19/2023
 *
 *     this file contains the functions to handle file opening
 *     and execution of instructions
 *
 **************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include <byteswap.h>
#include <uarray.h>
#include <assert.h>
#include "lilum.h"
#include "memory.h"
#include "instructions.h"

const int FAILURE = 1;

/*
    open_file
    ***************************************************************************
    Input:
        char *filename: character pointer to name of file that will be opened
    Returns:
        File pointer to open file
    Effects:
        Opens file of name filename. If the file is not successfully opened
        an error is printed to stderr and the program is exited.
    Expects:
        filename is not null
    ***************************************************************************
*/
FILE *open_file(char *filename)
{
        assert(filename != NULL);
        FILE *fp;
        fp = fopen(filename, "rb");
        if (fp == NULL)
        {
                fprintf(stderr, "%s: ", filename);
                fprintf(stderr, "No such file or directory\n");
                exit(FAILURE);
        }
        return fp;
}

/*
    read_instructions.c
    ***************************************************************************
    Input:
        FILE *input_file: pointer to open file
        Memory mem : Memory struct that holds the segments, free sequences,
                        and program counter
    Returns:
        none
    Effects:
        Read the contents of the file into 32-bit words and fills segment0 with
        those words
    Expects:
    ***************************************************************************
*/
void read_instructions(FILE *input_file, Memory mem)
{
        while (!feof(input_file))
        {
                int parta = getc(input_file);
                if (parta == EOF)
                {
                        break;
                }
                int partb = getc(input_file);
                if (partb == EOF)
                {
                        break;
                }
                int partc = getc(input_file);
                if (partc == EOF)
                {
                        break;
                }
                int partd = getc(input_file);
                if (partd == EOF)
                {
                        break;
                }
                /* Reading in 8 bits at a time to create a 32-bit word */
                uint32_t word = 0;
                word = Bitpack_newu(word, 8, 24, (uint32_t)parta);
                word = Bitpack_newu(word, 8, 16, (uint32_t)partb);
                word = Bitpack_newu(word, 8, 8, (uint32_t)partc);
                word = Bitpack_newu(word, 8, 0, (uint32_t)partd);

                append_segment0(mem, word);
        }
        fclose(input_file);
}

/*
    execute.c
    ***************************************************************************
    Input:
        Memory mem : Memory struct that holds the segments, free sequences,
                        and program counter
    Returns:
        none
    Effects:
        executes instruction based on opcode from 32-bit word
    Expects:
        Memory struct pointer is not NULL
    ***************************************************************************
*/
void execute(Memory mem)
{
        /* loop and increment the counter, then execute each instruction */
        uint64_t word;
        uint32_t opcode;
        uint32_t rA;
        uint32_t rB;
        uint32_t rC;
        uint32_t value = 0;

        while (instructions_complete(mem))
        {
                word = (uint64_t)instruction(mem);
                opcode = (uint32_t)Bitpack_getu(word, 4, 28);

                /* Based on instruction, get information from 32-bit word */
                if (opcode == 13)
                {
                        rA = (uint32_t)Bitpack_getu(word, 3, 25);
                        value = (uint32_t)Bitpack_getu(word, 25, 0);
                }
                else
                {
                        rA = (uint32_t)Bitpack_getu(word, 3, 6);
                        rB = (uint32_t)Bitpack_getu(word, 3, 3);
                        rC = (uint32_t)Bitpack_getu(word, 3, 0);
                }
                /* Based on opcode from word execute inddicated instruction */
                switch (opcode)
                {
                case 0:
                        conditional_move(rA, rB, rC);
                        break;
                case 1:
                        segmented_load(rA, rB, rC, mem);
                        break;
                case 2:
                        segmented_store(rA, rB, rC, mem);
                        break;
                case 3:
                        addition(rA, rB, rC);
                        break;
                case 4:
                        multiplication(rA, rB, rC);
                        break;
                case 5:
                        division(rA, rB, rC);
                        break;
                case 6:
                        nand(rA, rB, rC);
                        break;
                case 7:
                        halt(mem);
                        break;
                case 8:
                        map_segment(mem, rB, rC);
                        break;
                case 9:
                        unmap_segment(mem, rC);
                        break;
                case 10:
                        output(rC);
                        break;
                case 11:
                        input(rC);
                        break;
                case 12:
                        load_program(mem, rB, rC);
                        break;
                case 13:
                        load_value(rA, value);
                        break;
                }
        }
}
