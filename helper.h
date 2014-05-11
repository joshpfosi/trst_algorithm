/*   File: helper.h
 *   By: Joshua Pfosi, Date: Sat May 10
 *   Last Updated: Sat May 10 21:52:18
 *
 *   Organized helper functions
 */

#include <stdlib.h>
#include <stdio.h>
#include "state_rep.h"

/* helper functions */
Angle  ang_btwn_angles         (Angle    a1,        Angle    a2);
Angle  ang_to_wind             (Angle    wind,      Angle    heading);
Angle  ang_btwn_positions      (Position pos1,      Position pos2);
double distance_btwn_positions (Position pos1,      Position pos2);
int is_upwind                  (Angle    wind_ang,  Angle    ang_to_waypt);
double degrees_to_radians      (double   deg);
double radians_to_degrees      (double   rad);
Angle standardize              (Angle    a);
FILE *open_or_abort            (char     *filename, char     *mode);


