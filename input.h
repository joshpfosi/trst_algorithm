/*   File: input.h
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Sun Mar 23 09:27:28
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

/* Args: File ptr and initialized array of size MAX_WAYPTS
 * Purpose: Populates array w/ waypts read from file
 * Returns # of waypts read
 */
unsigned read_waypts(FILE *fp, Position *waypts, unsigned size);

/* prints results of algorithm to stdout */
void output_state(FILE *fp, Env_data env, Boat_data boat);

/* various print functions for structs */
void print_nav(Navigator nav);
void print_env(Env_data env);
void print_boat(Boat_data boat);
void print_position(Position pos);

/* Returns file pointer to filename and aborts otherwise w/ msg */
FILE *open_or_abort(char *filename, char *mode);

#endif /* INPUT_H */
