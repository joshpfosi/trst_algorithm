/*   File: input.c
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Sat Mar 22 10:43:54
 *
 *  file for reading in input as specified in input_format.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "state_rep.h"

/* reads a data string into boat and enviroment data */
int update_state(char *data, Env_data env, Boat_data boat)
{
    /* read env */
    int re = sscanf(data, DATA_FORMAT_STRING, DATA_ARGS);

    /* Skip badly formed lines */
    if (re != NUM_MEMS) {
        fprintf(stderr, "Error reading data, read in: %d / %d necessary "
                "members\n", re, NUM_MEMS);
        return 1;
    }

    return 0;
}

FILE *open_or_abort(char *filename, char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        perror(filename);
        exit(1);
    }
    return fp;
}
