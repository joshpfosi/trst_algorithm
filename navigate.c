/*   File: high_level.c
 *   By: Joshua Pfosi, Date: Fri Mar 21
 *   Last Updated: Thu Apr 24 20:32:06
 *
 *   Implementation of navigator for algorithm
 *   Takes in input from sensor, parsed by main.c in a loop and decides
 *   navigation related problems
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "input.h"
#include "state_rep.h"

#define MAX_WAYPTS 10
#define GROOVE 5 /* tolerance to be off exact course == 5 degrees */
/* #define TACK_THRESHOLD 45 */
#define CLOSE_HAULED_ANGLE 50

int skipper(Navigator nav);
Angle ang_btwn_positions(Position pos1, Position pos2);
Angle ang_btwn_angles(Angle theta, Angle phi);
Angle ang_to_wind(Angle wind, Angle heading);
int adjust_heading(Navigator nav, Angle off);
int adjust_sails(Angle to_wind);
void trim_sail(Angle trim); /* trim between max and min trim */
int adjust_rudder(int rudder_angle);
void update_pid(Rudder_PID_data pid, Angle error);
int is_upwind(Angle angle_to_wind, Angle ang_to_waypt);

//static inline float degrees_to_radians(float deg)
//{
//    return M_PI * deg / 180;
//}

static inline float radians_to_degrees(float rad)
{
    return 180 * rad / M_PI;
}

/* Args: Viable file for sensor input
 * Purpose: Parse input and pass data to skipper
 * Returns 0 if no errors, otherwise nonzero
 */
int read_data(FILE *input) {

    /* holds entire state of navigator - passed to pilot */
    Navigator nav = malloc(sizeof(*nav));
    nav->env = malloc(sizeof(*(nav->env)));
    /* note: may need to initialize the Position inner struct */
    nav->boat = malloc(sizeof(*(nav->boat)));
    nav->boat->PID = malloc(sizeof(*(nav->boat->PID)));
    nav->boat->PID->integral = 0;
    nav->boat->PID->pos = 0;
    nav->waypts = malloc(MAX_WAYPTS * sizeof(*(nav->waypts)));
    nav->current_waypt = 0;
    nav->num_waypts = read_waypts(input, nav->waypts, MAX_WAYPTS);

    int status = 0;
    char *line = NULL;
    size_t len = 0;

    /* reads in env and boat data */
    /* may replace getline with fgets in future */
    while (getline(&line, &len, input) != -1) {
        if (update_state(line, nav->env, nav->boat) == 0) { 
            if ((status = skipper(nav)) != 0) { /* unresolvable issue */
                return status;
            }
            output_state(stdout, nav->env, nav->boat);
        }
    }

    free(line);
    /* possibly leaking mem due to Position */
    free(nav->waypts);
    free(nav->boat);
    free(nav->env);
    free(nav);

    return 0;
}

/* Args: Navigator as parsed from file
 * Purpose: Assesses states and calls library functions to guide robotic boat
 * Returns 0 iff encountered no unresolvable problems */
int skipper(Navigator nav) {

    Angle ang_to_waypt  = ang_btwn_positions(nav->boat->pos, 
            nav->waypts[nav->current_waypt]);
    fprintf(stderr, "ang_to_waypt == %f\n", ang_to_waypt); /* debugging */

    Angle wind_ang      = nav->env->wind_dir;
    /* TODO if wind_ang is relative to boat, set heading = 0 */
    Angle heading       = nav->boat->heading;
    Angle angle_to_wind = abs(ang_to_wind(wind_ang, heading));
    Angle ideal_ang     = ang_to_waypt;
    /* if the waypt is in the No-Go-Zone then we calculate the closer tack, and
     * set that to our "ideal" heading */
    if (is_upwind(wind_ang, ang_to_waypt) == 0) {
        if (ang_btwn_angles(wind_ang, heading) > 0) {
            ideal_ang = wind_ang + CLOSE_HAULED_ANGLE;
        } else {
            ideal_ang = wind_ang - CLOSE_HAULED_ANGLE;
        }
    }

    Angle error = ang_btwn_angles(ideal_ang, heading);
    /* if heading adjustment impossible w/o irons / tack / gype */
    if (adjust_heading(nav, error) || adjust_sails(angle_to_wind))
        return 1; /* one of the calls couldn't recover */

    /* return 0 if everything was resolvable */
    return 0;
}

/* Args: ang_to_wind, ang_to_waypt
 * Purpose:
 * Returns 0 iff waypt is upwind, defined by CLOSE_HAULED_ANGLE
 */
int is_upwind(Angle ang_to_wind, Angle ang_to_waypt) {
    return !(abs(ang_btwn_angles(ang_to_wind, ang_to_waypt) < CLOSE_HAULED_ANGLE));
}

/* Purpose: Assess current heading and waypoint and adjust accordingly 
 * Returns 0 if successful, nonzero otherwise
 */
int adjust_heading(Navigator nav, Angle error) {
    float PROPORTIONAL_CONST = 0.2; // low level for minor adjustments
    float INTEGRAL_CONST = 0.0;
    Rudder_PID_data pid = nav->boat->PID;
    
    /* call the tack function */
    /*if (error >= TACK_THRESHOLD) {
        
        int rudder_angle = PROPORTIONAL_CONST * error + INTEGRAL_CONST * pid->integral;
        adjust_rudder(rudder_angle);
    }
    else */
    /* commented out because they were the same. if we find we need a different 
     * preportional constant later for tacking we can implement it */

    if (abs(error) >= GROOVE) {
        /* adjust heading */
        int rudder_angle = PROPORTIONAL_CONST * error + INTEGRAL_CONST * pid->integral;
        adjust_rudder(rudder_angle);
    }
    update_pid(pid, error);
#ifdef DATA_GEN
    fprintf(stderr, "heading == %f\n", nav->boat->heading);
    fprintf(stderr, "error == %f\n", error);
    nav->boat->heading -= error;
#endif
    return 0;
}

/* Purpose: Assess sail trim and wind and adjust accordingly 
 * Returns 0 if successful, nonzero otherwise
 * currently always returns 0
 */
const unsigned MIN_TRIM = 0;
const unsigned MAX_TRIM = 90;
int adjust_sails(Angle to_wind) {
    /* 45 < to_wind < 180 */
    /* MIN_TRIM < trim < MAX_TRIM */
    Angle trim = (MAX_TRIM - MIN_TRIM) / (180 - CLOSE_HAULED_ANGLE)
                                       * (to_wind - CLOSE_HAULED_ANGLE)
                                       + MIN_TRIM;
    if (trim < MIN_TRIM) {
        trim = MIN_TRIM;
    }
/*    trim_sail(trim);*/
    return 0;
}

/* Purpose: Call function to adjust rudder position.
 * Returns 0 if successful, nonzero otherwise
 */
int adjust_rudder(int rudder_angle) {
    (void) rudder_angle;
    //position(rudder_angle); //call function written by builders to move the rudder
    return 0;
}

/* Purpose: updates the integral part of our pid control
 */
void update_pid(Rudder_PID_data pid, Angle error) {
    if(pid->pos > ERROR_HISTORY_CAP){
        pid->integral -= pid->prev_errors[pid->pos];    
    }
    pid->integral += error;
    pid->prev_errors[pid->pos] = error;
    pid->pos = (pid->pos++) % ERROR_HISTORY_CAP;
}


/* Args: two Positions
 * Purpose: Calculate the angle defined by pos1 and pos2, using pos1 as origin
 * Returns the angle in standard position between 0 and 360
 */
Angle ang_btwn_positions(Position pos1, Position pos2) {
    fprintf(stderr, "pos1 %f, %f pos2 %f, %f\n", pos1.lat, pos1.lon, pos2.lat, pos2.lon);
    Angle ang_in_rads = atan2(pos2.lat - pos1.lat, pos2.lon - pos1.lon);
    Angle ang_in_degs = radians_to_degrees(ang_in_rads);
    if (ang_in_degs < 0) {
        ang_in_degs += 360;
    }
    return ang_in_degs;
}

/* Args: two Vectors
 * Purpose: Calculate the angle between to vectors
 * Returns the angle 
 */
Angle ang_btwn_angles(Angle a1, Angle a2) {
    return a2 - a1;
}
/* Args: two directions
 * Purpose: determine angle to wind for sail trim 
 * Notes: starboard is positive angle, port is negative angle
 *        angle returned (ar): -180 < ar <= 180
 */
Angle ang_to_wind(Angle wind, Angle heading) {
        Angle a = heading - wind;
        if (a > 180) {
                a -= 360;
        }
        return a;
}

