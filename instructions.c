/**************************************************************
 *
 *                     instructions.c
 *
 *     Assignment: um
 *     Authors:  Youssed Ezzo (yezzo01), Kerwin Teh (kteh01)
 *     Date:     11/19/2023
 *
 *     this file contains the functions to handle all UM
 *     instructions
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
#include <math.h>
#include "instructions.h"
#include "memory.h"

uint32_t registers[8] = {0};

/*
    conditional_move
    ***************************************************************************
    Input:
        uint32_t rA: value of A from unpacked 32-bit instruction
        uint32_t rB: value of B from unpacked 32-bit instruction
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        copies value in $r[B] into $r[A] only if $r[C] is not 0
    Expects:
        None
    ***************************************************************************
*/
void conditional_move(uint32_t rA, uint32_t rB, uint32_t rC)
{
    if (registers[rC] != 0)
    {
        registers[rA] = registers[rB];
    }
    return;
}

/*
    segmented_load
    ***************************************************************************
    Input:
        uint32_t rA: value of A from unpacked 32-bit instruction
        uint32_t rB: value of B from unpacked 32-bit instruction
        uint32_t rC: value of C from unpacked 32-bit instruction
        Memory mem: Memory struct containing all segments and free indices
    Returns:
        None
    Effects:
        Updates register A with the value stored at a specified location in
        memory segment[$rB][$rC]
    Expects:
        None
    ***************************************************************************
*/
void segmented_load(uint32_t rA, uint32_t rB, uint32_t rC, Memory mem)
{
    assert(mem != NULL);
    registers[rA] = value_in_segment(mem, registers[rB], registers[rC]);
}

/*
    segmented_store
    ***************************************************************************
    Input:
        uint32_t rA: value of A from unpacked 32-bit instruction
        uint32_t rB: value of B from unpacked 32-bit instruction
        uint32_t rC: value of C from unpacked 32-bit instruction
        Memory mem : Memory struct that holds the segments, free sequences,
                        and program counter
    Returns:
        None
    Effects:
        Stores value of $r[C] in memory at position [$r[A]][$r[B]]
    Expects:
        Memory struct pointer is not NULL
    ***************************************************************************
*/
void segmented_store(uint32_t rA, uint32_t rB, uint32_t rC, Memory mem)
{
    assert(mem != NULL);
    store_in_segment(mem, registers[rC], registers[rA], registers[rB]);
}

/*
    addition
    ***************************************************************************
    Input:
        uint32_t rA: value of A from unpacked 32-bit instruction
        uint32_t rB: value of B from unpacked 32-bit instruction
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        updates value in $r[A] with the sum of the values in $r[B] and $r[C]
    Expects:
        None
    ***************************************************************************
*/
void addition(uint32_t rA, uint32_t rB, uint32_t rC)
{
    registers[rA] = (uint32_t)((registers[rB] + registers[rC]));
}

/*
    multiplication
    ***************************************************************************
    Input:
        uint32_t rA: value of A from unpacked 32-bit instruction
        uint32_t rB: value of B from unpacked 32-bit instruction
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        updates value in $r[A] with the product of values in $r[B] and $r[C]
    Expects:
        None
    ***************************************************************************
*/
void multiplication(uint32_t rA, uint32_t rB, uint32_t rC)
{
    uint32_t rB_plz = registers[rB];
    uint32_t rC_plz = registers[rC];
    uint32_t ans = ((rB_plz * rC_plz));
    registers[rA] = ans;
}

/*
    division
    ***************************************************************************
    Input:
        uint32_t rA: value of A from unpacked 32-bit instruction
        uint32_t rB: value of B from unpacked 32-bit instruction
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        updates value in $r[A] with the quotient of values in $r[B] and $r[C]
    Expects:
        None
    ***************************************************************************
*/
void division(uint32_t rA, uint32_t rB, uint32_t rC)
{
    registers[rA] = (registers[rB] / registers[rC]);
}

/*
    nand
    ***************************************************************************
    Input:
        uint32_t rA: value of A from unpacked 32-bit instruction
        uint32_t rB: value of B from unpacked 32-bit instruction
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        updates value in $r[A] with the result of bitwise nand of the values
        in $r[B] and $r[C]
    Expects:
        None
    ***************************************************************************
*/
void nand(uint32_t rA, uint32_t rB, uint32_t rC)
{
    registers[rA] = ~(registers[rB] & registers[rC]);
}

/*
    halt
    ***************************************************************************
    Input:
        Memory mem : Memory struct that holds the segments, free sequences,
                     and program counter
    Returns:
        None
    Effects:
        frees all allocated memory and exits the program
    Expects:
        Memory struct pointer is not NULL
    ***************************************************************************
*/
void halt(Memory mem)
{
    assert(mem != NULL);
    free_segments(mem);
    exit(0);
}

/*
    map_segment
    ***************************************************************************
    Input:
        Memory mem : Memory struct that holds the segments, free sequences,
                        and program counter
        uint32_t rB: value of B from unpacked 32-bit instruction
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        A new segment is created with a number of words equal to $r[C]
        index of newly mapped segment is
    Expects:
        Memory struct pointer is not NULL
    ***************************************************************************
*/
void map_segment(Memory mem, uint32_t rB, uint32_t rC)
{
    assert(mem != NULL);
    uint32_t num_words = registers[rC];
    // TODO remove prints
    // printf("numwords %u\n", num_words);
    uint32_t new_index = map_segment_helper(mem, num_words);
    // printf("index %u\n", new_index);
    registers[rB] = new_index;
    // printf("register[rB] %u\n", registers[rB]);
}

/*
    unmap_segment
    ***************************************************************************
    Input:
        Memory mem : Memory struct that holds the segments, free sequences,
                        and program counter
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        A new segment is created with a number of words equal to $r[C]

    Expects:
        Memory struct pointer is not NULL
    ***************************************************************************
*/
void unmap_segment(Memory mem, uint32_t rC)
{
    assert(mem != NULL);
    unmap_segment_helper(mem, registers[rC]);
}

/*
    output
    ***************************************************************************
    Input:
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        prints the value in $r[C] to stdout
    Expects:
        value in $r[C] is between 0 and 255
    ***************************************************************************
*/
void output(uint32_t rC)
{
    assert(registers[rC] <= 255);
    fprintf(stdout, "%c", (char)registers[rC]);
    fflush(stdout);
}

/*
    input
    ***************************************************************************
    Input:
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        gets input from stdin and stores it in $r[C]
    Expects:
        ASCII value of inputted character must range from 0-255
    ***************************************************************************
*/
void input(uint32_t rC)
{
    int c = fgetc(stdin);
    // TODO check cat.um eof
    // assert(0 <= c && c <= 255);
    registers[rC] = (uint32_t)c;

    /* TODO LOAD EVERY BIT IS 1*/
}

/*
    load_program
    ***************************************************************************
    Input:
        Memory mem : Memory struct that holds the segments, free sequences,
                        and program counter
        uint32_t rB: value of C from unpacked 32-bit instruction
        uint32_t rC: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        duplicates segment $r[B] and replaces segment 0 with that segment
    Expects:
        Memory struct pointer is not NULL
    ***************************************************************************
*/
void load_program(Memory mem, uint32_t rB, uint32_t rC)
{
    assert(mem != NULL);
    rB = registers[rB];
    rC = registers[rC];
    load_program_helper(mem, rB, rC);
}

/*
    load_value
    ***************************************************************************
    Input:
        uint32_t rA: value of C from unpacked 32-bit instruction
        uint32_t rB: value of C from unpacked 32-bit instruction
    Returns:
        None
    Effects:
        loads specified "value" in $r[A]
    Expects:
        ascii value of inputted character must range from 0-255
    ***************************************************************************
*/
void load_value(uint32_t rA, uint32_t value)
{
    registers[rA] = value;
}