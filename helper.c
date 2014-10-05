/*   File: helper.c
 *   By: Joshua Pfosi, Date: Sat May 10
 *   Last Updated: Mon Sep 15 11:13:27
 *
 *   Implements helper functions
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "helper.h"

/* Args: two Positions
 * Purpose: Calculate the angle defined by pos1 and pos2, using pos1 as origin
 * Returns the angle in standard position between 0 and 360
 */
inline Angle ang_btwn_positions(Position pos1, Position pos2) {
    Angle ang_in_rads = atan2(pos2.lat - pos1.lat, pos2.lon - pos1.lon);
    Angle ang_in_degs = radians_to_degrees(ang_in_rads);
    if (ang_in_degs < 0) {
        ang_in_degs += 360;
    }
    return ang_in_degs;
}

/* Args: two angles
 * Purpose: Calculate the angle between to angles
 * Returns the angle 
 */
inline Angle ang_btwn_angles(Angle a1, Angle a2) {
    Angle a = a2 - a1;
    a = (a > 0) ? a : -a;
    return (a < 180) ? a : 360 - a;
}

/* Args: two directions
 * Purpose: determine angle to wind for sail trim 
 * Notes: starboard is positive angle, port is negative angle
 *        angle returned (ar): -180 < ar <= 180
 */
inline Angle ang_to_wind(Angle wind, Angle heading) {
    Angle a = heading - wind;
    return (a < 180) ? a : a - 360;
}

inline double distance_btwn_positions(Position pos1, Position pos2) {
    return sqrt((pos1.lat - pos2.lat) * (pos1.lat - pos2.lat)
            + (pos1.lon - pos2.lon) * (pos1.lon - pos2.lon));
}

/* Args: ang_to_wind, ang_to_waypt
 * Purpose:
 * Returns 0 iff waypt is upwind, defined by CLOSE_HAULED_ANGLE
 */
int is_upwind(Angle wind_ang, Angle ang_to_waypt) {
    return ang_btwn_angles(wind_ang, ang_to_waypt) < CLOSE_HAULED_ANGLE;
}

inline double degrees_to_radians(double deg) {
    return (deg * M_PI) / 180;
}

inline double radians_to_degrees(double rads) {
    return (rads / M_PI) * 180;
}

/* converts angle to standard position */
inline Angle standardize(Angle a) {
    if (a < 0)
        a += 360;
    if (a > 360)
        a -= 360;
    return a;
}

FILE *open_or_abort(char *filename, char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        perror(filename);
        exit(1);
    }
    return fp;
}


