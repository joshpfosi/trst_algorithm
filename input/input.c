/*  input.c
 *  ALEX TONG C:11/3/14 M:11/3/14
 *
 *  file for reading in input as specified in input_format.h
 */    

#include "input.h"
#include "input_format.h"
#include <stdio.h>

/* reads a data string into boat and enviroment data */
int input_env (char *data, env_dat env)
{
    /* read env */
    int re = sscanf(data, env_format_string, env_args);

    /* TODO enter recovery mode */
    if (re != num_env_mems) {
        fprintf("error reading enviroment data, read in: %d members\n", re);
        return 1;
    }

    return 0;
}

/* reads a data string into boat data */
int input_boat(char *data, boat_dat boat)
{
    /* read boat data */
    int rb = sscanf(data, boat_format_string, boat_args);

    /* TODO enter recovery mode */
    if (rb != num_boat_mems) {
        fprintf("error reading boat data read in: %d members\n", rb);
        return 1;
    }

    return 0;
}
