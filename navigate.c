/*   File: high_level.c
 *   By: Joshua Pfosi, Date: Fri Mar 21
 *   Last Updated: Fri Mar 21 22:27:49
 *
 *   Implementation of navigator for algorithm
 *   Takes in input from sensor, parsed by main.c in a loop and decides
 *   navigation related problems
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "input.h"

#define MAX_WAYPTS 10

int pilot(Navigator nav);
unsigned read_waypts(FILE *fp, Position *waypts);

int navigate(FILE *input) {

    /* holds entire state of navigator - passed to pilot */
    Navigator nav = malloc(sizeof(*nav));
    nav->env = malloc(sizeof(*(nav->env)));
    /* note: may need to initialize the Position inner struct */
    nav->boat = malloc(sizeof(*(nav->boat)));
    nav->waypts = malloc(MAX_WAYPTS * sizeof(*(nav->waypts)));
    nav->current_waypt = 0;
    nav->num_waypts = read_waypts(input, nav->waypts);

    int status = 0;
    char *line = NULL;
    size_t len = 0;

    /* reads in env and boat data */
    /* may replace getline with fgets in future */
    while (getline(&line, &len, input) != -1) {     /* read in line fine */
        if (update_state(line, nav->env, nav->boat) == 0) {     /* read in data fine */
            if ((status = pilot(nav)) != 0) { /* unresolvable issue */
                return status;
            }
        }
    }

    free(line);
    /* possibly leaking mem due to nav/env/boat/waypoints/position */
    free(nav->waypts);
    free(nav->boat);
    free(nav->env);
    free(nav);

    return 0;
}

/* Args: File ptr and initialize array of size MAX_WAYPTS
 * Purpose: Populates array w/ waypts read from file
 * Returns # of waypts read
 */
unsigned read_waypts(FILE *fp, Position *waypts) {
    char *line = malloc(16);
    unsigned num_waypoints = (fgetc(fp) - 48), i;
    assert(num_waypoints >= 0 && num_waypoints <= MAX_WAYPTS);

    /* read in each waypt */
    for (i = 0; i < num_waypoints; ++i) {
        fscanf(fp, "%f,%f;", &(waypts[i].lat), &(waypts[i].lon));
    }

    free(line);
    return num_waypoints;
}

/* Args: Navigator as parsed from file
 * Purpose: Assesses states and calls library functions to guide robotic boat
 * Returns 0 iff encountered no unresolvable problems */
int pilot(Navigator nav) {

    (void)nav;
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
