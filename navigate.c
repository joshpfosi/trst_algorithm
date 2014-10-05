/*   File: high_level.c
 *   By: Joshua Pfosi, Date: Fri Mar 21
 *   Last Updated: Sun Sep 28 20:44:44
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
#include "helper.h"

static int  skipper(Navigator nav);
static int  set_course(Navigator nav, Angle ang_to_waypt);
static int  adjust_heading(Navigator nav, Angle off);
static int  adjust_sails(Navigator nav, Angle to_wind);
       void luff(Navigator nav);
static void update_pid(Rudder_PID_data pid, Angle error);

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

#ifdef DATA_GEN
    /* initial boat position will not be 0,0 most likely */
    /* this is right off the corner of the dock */
    nav->boat->pos.lat = 42.431555;
    nav->boat->pos.lon = -71.147501;
#endif

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
#ifdef DATA_GEN
            output_state(stdout, nav->env, nav->boat);
#endif
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
static int skipper(Navigator nav) {
    Position pos   = nav->boat->pos,
             waypt = nav->waypts[nav->current_waypt]; 

    /* update waypoints - if at end, start again */
    if (distance_btwn_positions(pos, waypt) < WAYPT_RADIUS)
        nav->current_waypt = (++nav->current_waypt)%nav->num_waypts;

    /* return 0 if everything was resolvable */
    return set_course(nav, ang_btwn_positions(pos, waypt));
}

static int set_course(Navigator nav, Angle ang_to_waypt) {
    Angle wind_ang      = nav->env->wind_dir,
          heading       = nav->boat->heading,
          angle_to_wind = abs(ang_to_wind(wind_ang, heading)),
          ideal_ang     = ang_to_waypt;

    /* if the waypt is in the No-Go-Zone then we calculate the closer tack, and
     * set that to our "ideal" heading */
    if (is_upwind(wind_ang, ang_to_waypt)) {
        fprintf(stderr, "wind_ang == %f, ang_to_waypt == %f\n\n\n\n", wind_ang, ang_to_waypt);
        /* compute angle btwn heading and wind */
        Angle temp = heading - wind_ang;
        temp = (temp < 180) ? temp : 180 - temp; 
        if (temp > 0) { /* 0 < temp <= 180 */
            ideal_ang = standardize(wind_ang + CLOSE_HAULED_ANGLE);
        } else {
            ideal_ang = standardize(wind_ang - CLOSE_HAULED_ANGLE);
        }
    }

    /* calculate error in heading and adjust heading and sails */
    Angle error = ideal_ang - heading;
    return adjust_heading(nav, error) || adjust_sails(nav, angle_to_wind);
}

/* Purpose: Assess current heading and waypoint and adjust accordingly 
 * Returns 0 if successful, nonzero otherwise
 */
static int adjust_heading(Navigator nav, Angle error) {
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
        (void)rudder_angle;
        /* adjust_rudder(rudder_angle); */
#ifdef DATA_GEN /* w/ real boat, above call to adjust_rudder should do this part */
        nav->boat->heading += error;
        nav->boat->rud_pos = 0; /* always stay straight as adjustment is instant */
#endif
    }
    update_pid(pid, error);

    return 0;
}

/* Purpose: Assess sail trim and wind and adjust accordingly 
 * Returns 0 if successful, nonzero otherwise
 * currently always returns 0
 */
static int adjust_sails(Navigator nav, Angle to_wind) {
    /* 45 < to_wind < 180 */
    /* MIN_TRIM < trim < MAX_TRIM */
    Angle trim = (float)(MAX_TRIM - MIN_TRIM) / (180 - CLOSE_HAULED_ANGLE)
                                       * (to_wind - CLOSE_HAULED_ANGLE)
                                       + MIN_TRIM;
    if (trim < MIN_TRIM) {
        trim = MIN_TRIM;
    }

#ifdef DATA_GEN
    nav->boat->sail_pos = trim;
#else 
    (void)nav;
#endif
/*    trim_sail(trim); */
    return 0;
}

/* Purpose: updates the integral part of our pid control
 */
static void update_pid(Rudder_PID_data pid, Angle error) {
    if(pid->pos > ERROR_HISTORY_CAP) {
        pid->integral -= pid->prev_errors[pid->pos];    
    }
    pid->integral += error;
    pid->prev_errors[pid->pos] = error;
    pid->pos = (pid->pos++) % ERROR_HISTORY_CAP;
}

void luff(Navigator nav) {
    Angle error = nav->env->wind_dir - nav->boat->heading;

    adjust_heading(nav, error); /* head up wind */
    adjust_sails(nav, MAX_TRIM);     /* sails all the way out */
}
