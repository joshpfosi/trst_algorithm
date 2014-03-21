/*   File: high_level.c
 *   By: Joshua Pfosi, Date: Fri Mar 21
 *   Last Updated: Fri Mar 21 18:32:22
 *
 *   Implementation of high level decision maker for algorithm
 *   Takes in input from sensor, parsed by main.c in a loop and decides
 *   navigation related problems
 */

#include "input.h"

/* Args: Environment and boat state as reported by sensor
 * Purpose: Assesses states and calls library functions to guide robotic boat
 * Returns 0 iff encountered no unresolvable problems */
int pilot(Env_data env, Boat_data boat) {

    (void)env; (void)boat;
    /* assess data and call library functions to 
     * mutate boat state accordingly */

    /* return 0 if everything was resolvable */
    return 0;
}
