/*   File: waypoint_expansion.h
 *   By: Alex Tong Date: Sat May 10
 *   Last Updated: Sun Apr 27 20:47:36
 */

typedef struct Waypoints *Waypoints;

Waypoints read_waypts(FILE *fp);
int expand_waypts(FILE *fp, Waypoints wp, double r);
int print_waypts(FILE *fp, Waypoints wp, double r);
