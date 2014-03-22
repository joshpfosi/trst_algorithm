/*   File: read_waypts_test.c
 *   By: Joshua Pfosi, Date: Fri Mar 21
 *   Last Updated: Fri Mar 21 22:28:48
 *
 *   Test suite for read_waypts
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_WAYPTS 10

/* stores lat/lon info in convenient package */
typedef struct Position {
    float lat;
    float lon;
} Position;

/* Args: File ptr and initialize array of size MAX_WAYPTS
 * Purpose: Populates array w/ waypts read from file
 * Returns # of waypts read
 */
unsigned read_waypts(FILE *fp, Position *waypts) {
    char *line = malloc(16);
    unsigned num_waypoints = (fgetc(fp) - 48), i;

    /* read how many waypts */
    assert(num_waypoints >= 0 && num_waypoints <= MAX_WAYPTS);

    /* read in each waypt */
    for (i = 0; i < num_waypoints; ++i) {
        fscanf(fp, "%f,%f;", &(waypts[i].lat), &(waypts[i].lon));
    }
    free(line);
    return num_waypoints;
}

int main()
{
    unsigned i;
    Position *waypoints = malloc(MAX_WAYPTS * sizeof(*waypoints));
    unsigned num_waypoints = read_waypts(stdin, waypoints);
    
    for (i = 0; i < num_waypoints; ++i) {
        fprintf(stderr, "Waypoint read: %f,%f;\n", 
                waypoints[i].lat, waypoints[i].lon);
    }
    fprintf(stderr, "Number read: %u\n", num_waypoints);

    free(waypoints);

    return 0;
}
