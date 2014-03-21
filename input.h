/*   File: input.h
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Fri Mar 21 19:08:41
 *
 *  header file for getting input data on robotic sailboat TRST project
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include "input_format.h"

/* stores the data read in in *read in* order,
   use define statements in input_format.h to use data */
typedef struct Env_data {
    float wind_dir, wind_speed, app_wind_dir, app_wind_speed;
} *Env_data;

/* stores the data read in in *read in* order,
   use define statements in input_format.h to use data */
typedef struct Boat_data {
    float rud_pos, sail_pos, lat, lon, heading;
} *Boat_data;

/* Args: semicolon delimited list of doubles, Env_data and Boat_data
 * Purpose: Populates env and boat with data
 * Returns 0 if successful, 1 otherwise
 */
int update_state(char *data, Env_data env, Boat_data boat);

/* Returns file pointer to filename and aborts otherwise w/ msg */
FILE *open_or_abort(char *filename, char *mode);

#endif /* INPUT_H */
