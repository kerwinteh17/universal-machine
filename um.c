/**************************************************************
 *
 *                            um.c
 *
 *     Assignment: um
 *     Authors:  Youssed Ezzo (yezzo01), Kerwin Teh (kteh01)
 *     Date:     11/19/2023
 * 
 *     This file contains the main program that runs the Universal 
 *     Machine 
 *
 **************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include <sys/stat.h>
#include "memory.h"
#include "lilum.h"

/*
    main 
    ***************************************************************************
    Input: 
        int argc:     number of arguments passed into command line
        char *argv[]: character string of arguments passed into command line
    Returns: 
        TODO
    Effects: 
        Read and execute instructions from .um file passed into the
        command line or standard input
    Expects:
        argc > 0 
        argv is not NULL
    Notes: 
        Closes file pointer
    ***************************************************************************
*/
int main(int argc, char *argv[]){
        (void) argc;
        FILE *input_file = open_file(argv[1]);
        struct stat file_status;
        stat(argv[1], &file_status);
        long hint = file_status.st_size / 4;

        Memory mem = create_segment0(hint);
        read_instructions(input_file, mem);
        execute(mem);
}