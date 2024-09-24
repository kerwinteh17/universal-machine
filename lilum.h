/**************************************************************
 *
 *                     lilum.h
 *
 *     Assignment: um
 *     Authors:  Youssed Ezzo (yezzo01), Kerwin Teh (kteh01)
 *     Date:     11/19/2023
 * 
 *     lilum.h holds the definitions of the functions
 *     used in lilum.c
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
#include "memory.h"

#ifndef LILUM_H
#define LILUM_H


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
FILE * open_file(char *filename);

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
void read_instructions(FILE *input_file, Memory mem);

/*
    execute.c
    ***************************************************************************
    Input: 
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
void execute(Memory mem);

#endif