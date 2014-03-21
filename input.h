/*   File: input.h
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Fri Mar 21 18:39:37
 *
 *  header file for getting input data on robotic sailboat TRST project
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include "input_format.h"

/* CHANGE LOG
 *
 * [] Changed type names to C standard
 * [] Added my header cause I'm OCD
 */

/* stores the data read in in *read in* order,
   use define statements in input_format.h to use data */
typedef struct Env_data {
    float dat[num_env_mems];
} *Env_data;

/* stores the data read in in *read in* order,
   use define statements in input_format.h to use data */
typedef struct Boat_data {
    float dat[num_boat_mems];
} *Boat_data;

/* Args: semicolon delimited list of doubles, Env_data and Boat_data
 * Purpose: Populates env and boat with data
 * Returns 0 if successful, 1 otherwise
 */
int parse_line(char *data, Env_data env, Boat_data boat);

/* Returns file pointer to filename and aborts otherwise w/ msg */
FILE *open_or_abort(char *filename, char *mode);

#endif /* INPUT_H */
