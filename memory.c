/**************************************************************
 *
 *                     memory.c
 *
 *     Assignment: um
 *     Authors:  Youssed Ezzo (yezzo01), Kerwin Teh (kteh01)
 *     Date:     11/19/2023
 * 
 *     this file contains the functions that need access to the
 *     Memory struct. It contains all helper functions for 
 *     instructions.c. 
 *
 **************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include <assert.h>
#include "memory.h"


/* Definition of Memory struct that holds segments, free segments and the 
program counter */
struct Memory {
        Seq_T segment_sequence;
        Seq_T free_segments; 
        long program_counter;
};

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
Memory create_segment0(long hint){
        Memory mem = malloc(sizeof(struct Memory));
        assert(mem != NULL);
        mem->segment_sequence = Seq_new(0);
        mem->free_segments = Seq_new(0);
        Seq_T segment0 = Seq_new(hint);
        Seq_addhi(mem->segment_sequence, segment0);        
        mem->program_counter = 0;
        return mem;
}

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
void append_segment0(Memory mem, uint32_t word) {
        Seq_addhi(Seq_get(mem->segment_sequence, 0), (void *)(uintptr_t)word);
}

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
        If there is a free (unmapped) index, the segment is placed at that
        index; otherwise, the segment is added to the end of the sequence
    Expects: 
        memory struct pointer is not NULL
    ***************************************************************************
*/
uint32_t map_segment_helper(Memory mem, uint32_t num_words){
        Seq_T new_segment = Seq_new(num_words);
        /* Initialize sequence to all 0s*/
        for (uint32_t i = 0; i < num_words; i++) {
                Seq_addhi(new_segment, (void *)(uintptr_t)0);
        }
        /* If free_segments is empty, append the created segment to the end 
           of the sequence; otherwise, place the sequence at the free index 
           from free_segments */
        if (Seq_length(mem->free_segments) == 0){
             Seq_addhi(mem->segment_sequence, (void *)(uintptr_t)new_segment);
             return (uint32_t)(Seq_length(mem->segment_sequence) - 1);
        } else {
            int index = (int)(uintptr_t)Seq_remlo(mem->free_segments);
            Seq_put(mem->segment_sequence, index, 
                                            (void *)(uintptr_t)new_segment);
            return (uint32_t)index;
        } 

}

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
void unmap_segment_helper(Memory mem, uint32_t rC){
    Seq_T remove = Seq_get(mem->segment_sequence, rC);
    Seq_free(&remove);
    Seq_put(mem->segment_sequence, rC, NULL);
    Seq_addhi(mem->free_segments, (void *)(uintptr_t)rC);
}

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
        Creates a duplicate of segment[rB] and replaces segemnt 0 with that 
        segment.
        If the index being duplicated is not segment 0, segment 0 is freed.
        Program counter is set to rC
    Expects: 
        memory struct pointer is not NULL
    ***************************************************************************
*/
void load_program_helper(Memory mem, uint32_t rB, uint32_t rC) {
        if (rB != 0){
                Seq_T old = (Seq_T)Seq_get(mem->segment_sequence, rB);
                int length = Seq_length(old);
                Seq_T duplicate = Seq_new(length);
                /* Populate the duplicate segment with the elements in the 
                   original */
                for (int i = 0; i < length; i++) {
                        Seq_addhi(duplicate, 
                                          (void *)(uintptr_t)Seq_get(old, i));
                }
                Seq_T old_0 = (Seq_T)Seq_get(mem->segment_sequence, 0);
                Seq_free(&old_0);
                Seq_put(mem->segment_sequence, 0, duplicate);  
        }
        mem->program_counter = rC;
}


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
uint32_t value_in_segment(Memory mem, uint32_t indexB, uint32_t indexC){
        Seq_T segment = (Seq_T)Seq_get(mem->segment_sequence, indexB);
        uint32_t value = (uint32_t)(uintptr_t)Seq_get(segment, indexC);
        return value;
}


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
                        uint32_t indexB){
        Seq_T segment = (Seq_T)Seq_get(mem->segment_sequence, indexA);
        Seq_put(segment, indexB, (void *)(uintptr_t)value);
}


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
uint32_t instruction(Memory mem) {
        Seq_T seg0 = Seq_get(mem->segment_sequence, 0);
        uint32_t word = (uint32_t)(uintptr_t)Seq_get(seg0, 
                                                        mem->program_counter);
        mem->program_counter += 1;
        return word;
}


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
void free_segments(Memory mem) {
        int seq_length = Seq_length(mem->segment_sequence);
        /* Call Seq_free on all sequences withing or segments_sequence */
        for (int i = 0; i < seq_length; i++){
                Seq_T free_seq = Seq_get(mem->segment_sequence, i);
                if(free_seq != NULL){
                        Seq_free(&free_seq);
                }
        }       
        Seq_free(&(mem->segment_sequence));
        Seq_free(&(mem->free_segments));
        free(mem);
}


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
bool instructions_complete(Memory mem) {
        return mem->program_counter < 
                        Seq_length((Seq_T)(Seq_get(mem->segment_sequence, 0)));
}
