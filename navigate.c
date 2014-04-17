/*   File: high_level.c
 *   By: Joshua Pfosi, Date: Fri Mar 21
 *   Last Updated: Sun Mar 30 16:36:50
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
#define TACK_THRESHOLD 45 /* tolerance to not tack */

int skipper(Navigator nav);
Angle ang_btwn_positions(Position pos1, Position pos2);
Angle ang_btwn_angles(Angle theta, Angle phi);
int adjust_heading(Navigator nav, Angle off);
int adjust_sails(Navigator nav);
int adjust_rudder(int rudder_angle);
void update_pid(Rudder_PID_data pid, Angle error);

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

    Angle ang_to_waypt = ang_btwn_positions(nav->boat->pos, 
            nav->waypts[nav->current_waypt]);

    /* if heading adjustment impossible w/o irons / tack / gype */
    if (adjust_heading(nav, ang_to_waypt) || adjust_sails(nav))
        return 1; /* one of the calls couldn't recover */

    /* return 0 if everything was resolvable */
    return 0;
}

/* Purpose: Assess current heading and waypoint and adjust accordingly 
 * Returns 0 if successful, nonzero otherwise
 */
int adjust_heading(Navigator nav, Angle error) {

    Angle off = ang_btwn_angles(ang_to_waypt, nav->boat->heading);

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
    return 0;
}

/* Purpose: Assess sail trim and wind and adjust accordingly 
 * Returns 0 if successful, nonzero otherwise
 */
int adjust_sails(Navigator nav) {
    (void)nav;
    return 0;
}

/* Purpose: Call function to adjust rudder position.
 * Returns 0 if successful, nonzero otherwise
 */
int adjust_rudder(int rudder_angle) {
    //position(rudder_angle); //call function written by builders to move the rudder
    return 0;
}

/* Purpose: updates the integral part of our pid control
 */
void update_pid(Rudder_PID_data pid, Angle error) {
    if(pid->pos > ERROR_HISTORY_CAP){
        integral -= pid->prev_errors[pid->pos % ERROR_HISTORY_CAP];    
    }
    integral += error;
    pid->prev_errors[pid->pos % ERROR_HISTORY_CAP] = error;
}


/* Args: two Positions
 * Purpose: Calculate the angle defined by pos1 and pos2, using pos1 as origin
 * Returns the angle in standard position
 */
Angle ang_btwn_positions(Position pos1, Position pos2) {
    (void)pos1; (void)pos2;
    return 0;
}

/* Args: two Vectors
 * Purpose: Calculate the angle between to vectors
 * Returns the angle
 */
Angle ang_btwn_angles(Angle theta, Angle phi) {
    return theta - phi;
}
