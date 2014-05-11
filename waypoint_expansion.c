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
 *                     [p,s,n][lat],[long];...[n][lat],[long];
 *      Note: p for round to port, s for round to starboard, n for no round
 *   Output File spec: [num_wpts]\n
 *                     [lat],[long];...
 *
 *   TODO: does this run at up time or at enter time? The problem is getting
           angle to the first waypt.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "assert.h"
#include "state_rep.h"
#include "waypoint_expansion.h"

static inline double radians_to_degrees(double rad)
{
    return 180 * rad / M_PI;
}

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

struct Waypoints {
    char *rounding_dirs;
    Position *waypts;
    unsigned size;
};

Waypoints read_waypts(FILE *fp) {
    Waypoints wp = malloc(sizeof(*wp));
    unsigned i;
    assert(fscanf(fp, "%u", &(wp->size)) == 1);
    assert(wp->size >= 0);

    /* read in each waypt */
    for (i = 0; i < wp->size; i++) {
        assert(fscanf(fp, "%c%lf,%lf;", &(wp->rounding_dirs[i]),
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

/* partitions a1 and a2 into the smallest segments > SEG_SIZE
 * a1, a2, mod360 angles
 * dir: -1:starboard, 0:neither, 1:port
 */
int calc_divs(Angle a1, Angle a2, int dir) {
    /* port == true */
    int tmp = dir + 1 ? a2 - a1 : a1 - a2;
    int num_segs = ((int)mod360(tmp)) / SEG_SIZE;
    return (dir * mod360(tmp)) / num_segs;
}

/* prints a waypoint with a given position*/
void print_wpt_pos(FILE *fp, Position p) {
    assert(fp != NULL);
    fprintf(fp, "%f,%f;", p.lat, p.lon);
}

/* prints all waypoints disregarding rounding information, leaving waypoints
 * unexpanded
 * WARNING: unlike expand_waypts(FILE *, Waypoints) this function prints first
 *          waypoint
 */
int print_waypts(FILE *fp, Waypoints wp, double r) {
    assert(fp != NULL);
    (void) r;
    unsigned i = 0;
    for (i = 0 ; i < wp->size; i++) {
        print_wpt_pos(fp, wp->waypts[i]);
    }
    return 0;
}

/* given position, angle, radius, calculates a new position
 * radius must be given in lat long terms */
Position calc_wpt_from_pos_and_ang(Position p, Angle a, double r) {
    Position to_return;
    to_return.lat = p.lat + r * sinf(a);
    to_return.lon = p.lon + r * cosf(a);
    return to_return;
}

const int WAYPOINT_ERROR_FACTOR = 5; /* allowable error on produced waypoint
                                        angle */

int expand_waypts(FILE *fp, Waypoints wp, double r) {
    unsigned i = 0;
    Angle init_ang, fin_ang, ang_past, ang_toward;
    int dir, wp_sep;
    for (i = 1; i < wp->size-1; i++) {
        /* round mark to p[ort] s[tarbord] n[either] */
        char rc = wp->rounding_dirs[i];
        /* print angle and continue for case n */
        if (rc == 'n') {
            fprintf(fp, "%f,%f;", wp->waypts[i].lat, wp->waypts[i].lon);
            continue;
        }
        ang_past = ang_btwn_positions(wp->waypts[i], wp->waypts[i-1]);
        ang_toward = ang_btwn_positions(wp->waypts[i], wp->waypts[i+1]);
        init_ang = mod360(rc == 'p'? ang_past + 90 : ang_past + 270);
        fin_ang  = mod360(rc == 'p'? ang_toward + 270 : ang_toward + 90);

        /* rotate axis relative to the init_angle */
        /* currently implementing a "circle" as 20+ degree segments */
        dir = rc == 'p' ? 1 : -1; /* rc is already not 'n' */
        wp_sep = calc_divs(init_ang, fin_ang, dir);
        int tmp = init_ang;
        Position tmp_wp;
        while (abs(tmp - fin_ang) > WAYPOINT_ERROR_FACTOR) {
            tmp_wp = calc_wpt_from_pos_and_ang(wp->waypts[i], tmp, r);
            print_wpt_pos(fp, tmp_wp);
        }
    }
    assert(fprintf(fp, "%f,%f;", wp->waypts[i].lat, wp->waypts[i].lon) == 2);

    return 0;
}

