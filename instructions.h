/**************************************************************
 *
 *                     instructions.h
 *
 *     Assignment: um
 *     Authors:  Youssed Ezzo (yezzo01), Kerwin Teh (kteh01)
 *     Date:     11/19/2023
 * 
 *     instructions.h holds the definitions of the functions
 *     used in instructions.c
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


extern uint32_t registers[8];


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
void conditional_move(uint32_t rA, uint32_t rB, uint32_t rC);

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
        memory m[$rB][$rC]
    Expects: 
        None
    ***************************************************************************
*/
void segmented_load(uint32_t rA, uint32_t rB, uint32_t rC, Memory mem);

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
void segmented_store(uint32_t rA, uint32_t rB, uint32_t rC, Memory mem);

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
void addition(uint32_t rA, uint32_t rB, uint32_t rC);

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
void multiplication(uint32_t rA, uint32_t rB, uint32_t rC);

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
void division(uint32_t rA, uint32_t rB, uint32_t rC);

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
void nand(uint32_t rA, uint32_t rB, uint32_t rC);

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
void halt(Memory mem);

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
void map_segment(Memory mem, uint32_t rB, uint32_t rC);

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
void unmap_segment(Memory mem, uint32_t rC);

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
void output(uint32_t rC);

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
void input(uint32_t rC);

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
void load_program(Memory mem, uint32_t rB, uint32_t rC);

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
void load_value(uint32_t rA, uint32_t value);