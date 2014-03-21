/*   File: input.c
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Fri Mar 21 18:57:13
 *
 *  file for reading in input as specified in input_format.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "input_format.h"

/* reads a data string into boat and enviroment data */
int parse_line(char *data, Env_data env, Boat_data boat)
{
    /* read env */
    int re = sscanf(data, data_format_string, data_args);

    /* Skip badly formed lines */
    if (re != num_mems) {
        fprintf(stderr, "Error reading data, read in: %d / %d necessary "
                "members\n", re, num_mems);
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
