/*   File: input.h
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Sat Mar 22 10:39:06
 *
 *  header file for getting input data on robotic sailboat TRST project
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include "state_rep.h"

/* Args: semicolon delimited list of doubles, Env_data and Boat_data
 * Purpose: Populates env and boat with data
 * Returns 0 if successful, 1 otherwise
 */
int update_state(char *data, Env_data env, Boat_data boat);

/* Returns file pointer to filename and aborts otherwise w/ msg */
FILE *open_or_abort(char *filename, char *mode);

#endif /* INPUT_H */
