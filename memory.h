/**************************************************************
 *
 *                     memory.h
 *
 *     Assignment: um
 *     Authors:  Youssed Ezzo (yezzo01), Kerwin Teh (kteh01)
 *     Date:     11/19/2023
 * 
 *     memory.h holds the definitions of the functions
 *     used in memory.c
 *
 **************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include <byteswap.h>
#include <uarray.h>
#include <stdbool.h>

#ifndef MEMORY_H
#define MEMORY_H


typedef struct Memory *Memory; 

/*
    create_segment0
    ***************************************************************************
    Input: 
        long hint : hint to indicate potential size of sequence
    Returns:
        pointer to Memory struct holding memory segments, free indices, 
        and program counter
    Effects:
        Allocate space for Memory struct and initialize the elements of struct
    Expects: 
    ***************************************************************************
*/
Memory create_segment0();


/*
    append_segment0
    ***************************************************************************
    Input: 
        Memory mem : Memory struct that holds the segments, free sequences, 
                        and program counter
        uint32_t word: 32 bit words holding instruction and register values
    Returns:
        none
    Effects:
        Adds the passed in word to segment0
    Expects: 
    ***************************************************************************
*/
void append_segment0(Memory mem, uint32_t word);

/*
    map_segment_helper
    ***************************************************************************
    Input: 
        Memory mem : Memory struct that holds the segments, free sequences, 
                        and program counter
        uint32_t num_words: number of words the sequenc will hold
    Returns:
        uint32_t holding the index of the mapped segment
    Effects:
        Creates a new memory segment and adds it to the sequence of segments.
    Expects: 
        memory struct pointer is not NULL
    ***************************************************************************
*/
uint32_t map_segment_helper(Memory mem, uint32_t num_words);

/*
    unmap_segment_helper
    ***************************************************************************
    Input: 
        Memory mem : Memory struct that holds the segments, free sequences, 
                        and program counter
        uint32_t rC: value in rC indicating which segment is being unmapped
    Returns:
        none
    Effects:
        Replaces the segment at the specified index with a NULL pointer 
        and appends the (now free) index to the free_segments sequence
    Expects: 
        memory struct pointer is not NULL
    ***************************************************************************
*/
void unmap_segment_helper(Memory mem, uint32_t rC);

/*
    load_program_helper
    ***************************************************************************
    Input: 
        Memory mem : Memory struct that holds the segments, free sequences, 
                        and program counter
        uint32_t rB: value in register B indicating which segment will be
                     duplicated
        uint32_t rC: value in register C indicating which instruction in the
                     segment is meant to be executed
    Returns:
        none
    Effects:
        
    Expects: 
        memory struct pointer is not NULL
    ***************************************************************************
*/
void load_program_helper(Memory mem, uint32_t rB, uint32_t rC);

/*
    value_in_segment
    ***************************************************************************
    Input: 
        Memory mem : Memory struct that holds the segments, free sequences, 
                        and program counter
        uint32_t indexB : value in $r[B] representing the segmentID
        uint32_t indexC : value in $r[C] representing the address in segment 
    Returns:
        value in segment[indexB][indexC]
    Effects:
        None
    Expects: 
        Memory struct pointer is not NULL
    ***************************************************************************
*/
uint32_t value_in_segment(Memory mem, uint32_t indexB, uint32_t indexC);

/*
    store_in_segment
    ***************************************************************************
    Input: 
        Memory mem : Memory struct that holds the segments, free sequences, 
                        and program counter
        uint32_t value  : value in $r[C]
        uint32_t indexA : value in $r[A] representing the segmentID
        uint32_t indexB : value in $r[B] representing the address in segment 
    Returns:
        None
    Effects:
        Stores 'value' in segment[indexA][indexB]
    Expects: 
        Memory struct pointer is not NULL
    ***************************************************************************
*/
void store_in_segment(Memory mem, uint32_t value, uint32_t indexA, 
                                                            uint32_t indexB);

/*
    instruction
    ***************************************************************************
    Input: 
        Memory mem : Memory struct that holds the segments, free sequences, 
                        and program counter
    Returns:
        32-bit instruction located at program_counter in segment0. 
    Effects:
        Extracts the 32-bit word from segment0 and increments the program 
        counter by 1, indicating that the instruction is processed. 
    Expects: 
        Memory struct pointer is not NULL
    ***************************************************************************
*/
uint32_t instruction(Memory mem);

/*
    free_segments 
    ***************************************************************************
    Input: 
        Memory mem : Memory struct that holds the segments, free sequences, 
                        and program counter
    Returns:
        None
    Effects:
        Frees all segments, free_sequences, and the sequence that holds the 
        segments. All allocated memory is deallocated in this function,
        including the Memory struct
    Expects: 
        Memory struct pointer is not NULL
    ***************************************************************************
*/
void free_segments(Memory mem);


/*
    instructions_complete
    ***************************************************************************
    Input: 
        Memory mem : Memory struct that holds the segments, free sequences, 
                        and program counter
    Returns:
        True if the program counter is less than the number of instructions 
        in segment0, False otherwise. 
    Effects:
        None 
    Expects: 
        Memory struct pointer is not NULL
    ***************************************************************************
*/
bool instructions_complete(Memory mem);


#endif