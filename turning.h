/*   File: turning.h
 *   By: Joshua Pfosi, Date: Mon Mar 31
 *   Last Updated: Mon Mar 31 10:19:20
 *
 *   Defines functions used by navigate.c to turn, tack, and gybe the boat
 */

#ifndef TURNING_H
#define TURNING_H

#include "state_rep.h"

/* takes in the machine state and the angle off the current heading to turn and
 * moves the rudder accordingly
 */
int turn(Navigator nav, Angle off);

/* takes in the machine state, figures out in which direction a tack is 
 * reasonable, and executes the tack, returning to a straight line path
 */
int tack(Navigator nav);

/* same as tack but for a gybe */
int gybe(Navigator nav);

#endif
