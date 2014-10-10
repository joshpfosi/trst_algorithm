


#include <stdio.h>
#include "waypoint_expansion.h"



int main(int argc, char *argv[]) {
    
    (void) argc;
    (void) argv;
    FILE *fp = fopen("wptest", "r");
    Waypoints wp = read_waypts(fp);
    FILE *out = fopen("original", "w");
    print_waypts(out, wp, 1);
    FILE *out2 = fopen("expansion", "w");
    expand_waypts(out2, wp, 1);

    return 0;
}
