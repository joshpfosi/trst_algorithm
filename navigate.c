/*   File: high_level.c
 *   By: Joshua Pfosi, Date: Fri Mar 21
 *   Last Updated: Sun Mar 23 09:45:43
 *
 *   Implementation of navigator for algorithm
 *   Takes in input from sensor, parsed by main.c in a loop and decides
 *   navigation related problems
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "input.h"
#include "state_rep.h"

#define MAX_WAYPTS 10

int skipper(Navigator nav);
Angle ang_btwn_positions(Position pos1, Position pos2);
Angle ang_btwn_angles(Angle theta, Angle phi);
int adjust_heading(Navigator nav);
int adjust_sails(Navigator nav);

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
    if (!adjust_heading(nav)) {
        Angle off = ang_btwn_angles(ang_to_waypt, nav->boat->heading);
        /* if (some_low < off < sum_high) {
                tack();
           }
        */
        (void)off;
    }

    adjust_sails(nav);
    /* assess data and call library functions to 
     * mutate boat state accordingly */


    /* -- example pseudo-code */

    /* switch on lat/lon
     *      case <reached waypoint>:
     *               nav->current_waypoint++;
     *               break;
     */

    /* return 0 if everything was resolvable */
    return 0;
}

/* Purpose: Assess current heading and waypoint and adjust accordingly 
 * Returns 0 if successful, nonzero otherwise
 */
int adjust_heading(Navigator nav) {
    (void)nav;
    return 0;
}

/* Purpose: Assess sail trim and wind and adjust accordingly 
 * Returns 0 if successful, nonzero otherwise
 */
int adjust_sails(Navigator nav) {
    (void)nav;
    return 0;
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
