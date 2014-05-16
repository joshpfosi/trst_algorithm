/*   File: waypoint_expansion.h
 *   By: Alex Tong Date: Sat May 10
 *   Last Updated: Sun Apr 27 20:47:36
 */

typedef struct Waypoints *Waypoints;

/* given a properly formated input file, reads waypoints into a waypoint
 * struct */
Waypoints read_waypts(FILE *fp);

/* given an output file, a waypoint struct, and a radius, prints
 * a program readable waypoint list */
int expand_waypts(FILE *fp, Waypoints wp, double r);

/* prints waypoints to file, stripping rounding and radius information
 * equivalent to a file of waypoints all specified by n[either] */
int print_waypts(FILE *fp, Waypoints wp, double r); 


