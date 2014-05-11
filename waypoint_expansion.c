/*   File: waypoint_expansion.c
 *   By: Alex Tong and Morgan Gellert, Date: Wed May 7
 *   Last Updated: Sun Apr 27 20:47:36
 *
 *   Expands a list of human waypoints into boat readable waypoints
 *   More specifically, given radius, and input file containing waypoints
 *   and rounding directions, outputs a series of waypoints for the boat to go
 *   around the given waypoint.
 *
 *   Input File specs: [num_wpts]\n
 *                     [n][lat],[lon];[p,s,n][lat],[lon];...[n][lat],[lon];
 *      Note: p for round to port, s for round to starboard, n for no round
 *            begin and end the file with the start and finish points for the boat
 *              and denote them with n (no round)
 *   Output File spec: [num_wpts]\n
 *                     [lat],[lon];...
 *
 *   TODO: does this run at up time or at enter time? The problem is getting
           angle to the first waypt.
 */

#include <stdio.h>
#include <math.h>
#include "state_rep.h"

/* Args: two Positions
 * Purpose: Calculate the angle defined by pos1 and pos2, using pos1 as origin
 * Returns the angle in standard position between 0 and 360
 */
Angle ang_btwn_positions(Position pos1, Position pos2) {
    Angle ang_in_rads = atan2(pos2.lat - pos1.lat, pos2.lon - pos1.lon);
    Angle ang_in_degs = radians_to_degrees(ang_in_rads);
    if (ang_in_degs < 0) {
        ang_in_degs += 360;
    }
    return ang_in_degs;
}

/* Turns a degree representation of an "Angle" into a value between 0 and 360
 * NOTE: slow implementation, if dealing with very large angles must change
 *       algorithm
 */
Angle mod360(Angle a1) {
    while (a1 < 0) {
        a1 += 360;
    }
    while (a1 >= 360) {
        a1 -= 360;
    }
    return a1;
}
 

/* Args: two Vectors
 * Purpose: Calculate the angle between to vectors
 * Returns the angle, between 0 and 360
 */
Angle ang_btwn_angles(Angle a1, Angle a2) {
    Angle to_return = a2 - a1;
    mod360(to_return);
    return to_return;
}

/* Waypoint structure for abstraction
 */

typedef struct Waypoints {
    char *rounding_dirs;
        /* n for no round, p and s for port and starboard */
    Position *waypts;
    unsigned size;
}* Waypoints;

Waypoints read_waypts(FILE *fp) {
    Waypoints wp = malloc(sizeof(*wp));

    assert(fscanf(fp, "%u", &(wp->size) == 1));
    assert(wp->size >= 0 && wp->size <= size);

    /* read in each waypt */
    unsigned i;
    for (i = 0; i < wp->size; i++) {
        assert(fscanf(fp, "%c%f,%f;", &(wp->rounding_dirs[i]),
                                      &(wp->waypts[i].lat),
                                      &(wp->waypts[i].lon)) == 3);
        assert(wp->rounding_dirs[i] == 'p' ||
               wp->rounding_dirs[i] == 's' ||
               wp->rounding_dirs[i] == 'n');
    }
    fgetc(fp); /* discard the newline */
    return wp;
}

/* the size in degrees of the min segmenet size */
const unsigned SEG_SIZE = 20;

/* radius to go around the waypoint in meters */
const float TURN_RADIUS = 3.0;

/* partitions a1 and a2 into the smallest segments > SEG_SIZE
 * alpha, beta, mod360 angles
 * dir: -1:starboard, 0:neither, 1:port
 *      note: 0 should never happen in current implementation
 * then:
 * prints the waypoints given waypoint and direction and 
 *          earlier calculated information
 *      note: step size is signed so it will go in the correct direction
 */

int print_expand_wpt(FILE *fp, 
                     Angle alpha, Angle beta, Angle step, 
                     Position base, char dir) {

    int int_dir;

    if(dir == 'p'){
        int_dir = 1;
    }
    else if(dir == 's'){
        int_dir = -1;
    }
    else{
        /* we have a problem */
        return 1;
    }

    Angle to_use = int_dir == 1 ? ang_btwn_angles(alpha, beta) : 
                                  ang_btwn_angles(beta, alpha);
    int num_segs = to_use / SEG_SIZE;
    Angle step_ang = (int_dir * to_use) / num_segs;


    Position temp;

    unsigned i;
    for(i = 0, i < num_segs, i ++) {
        temp.lat = base.lat + cos(alpha + i * step_ang);
        temp.lon = base.lon + sin(alpha + i * step_ang);

        fprintf(fp, "%f,%f;", temp.lat, temp.lon);
    }
    return 0;
}

int expand_waypts(FILE *fp, Waypoints wp) {
    int error_status = 0;
    unsigned i = 0;
    Angle init_ang, fin_ang, ang_past, ang_toward;
    for (i = 1; i < wp->size-1; i++) {
        /* round mark to p[ort] s[tarbord] n[either] */
        char rc = rounding_dirs[i];
        /* print angle and continue for case n */
        if (rc == 'n') {
            fprintf(fp, "%f,%f;", wp->waypts[i].lat, wp->waypts[i].lon);
            continue;
        }

        ang_past = ang_btwn_positions(wp->waypts[i], wp->waypts[i-1]);
        ang_toward = ang_btwn_positions(wp->waypts[i], wp->waypts[i+1]);
        init_ang = mod360(rc == 'p'? ang_past + 90 : ang_past + 270);
        fin_ang  = mod360(rc == 'p'? ang_toward + 270 : ang_toward + 90);


        /* currently implementing a "circle" as 20+ degree segments */
        /*Angle wp_step = calc_divs(init_ang, fin_ang, dir);*/

        error_status += print_expand_wpt(fp, init_ang, fin_ang, wp->waypts[i], dir);
    }
    assert(fprintf(fp, "%f,%f;", wp->waypts[i].lat, wp->waypts[i].lon) == 2);


    return error_status;
}


int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    int exit_status = 0;

    /* reads in data until there is an error */
    if (argc > 2) { /* cannot handle more than 1 argument */
        fprintf(stderr, "%s: usage: [filename] [radius] \n", argv[0]);
        exit(1);
    }
    else if (argc == 1) /* no filename was given so use stdin */
        fp = stdin; 
    else /* num_args == 2 so use filename */
        fp = open_or_abort(argv[1], "r");

    /* start main program */
    Waypoints wp = read_waypts(input, nav->waypts, MAX_WAYPTS);

    exit_status += expand_waypts(fp, wp);

    fclose(fp);

    if (exit_status != 0) {
        fprintf(stderr, "Aborting due to unresolvable input error\n");
    }
    return exit_status;
}
