#include <stdio.h>
#include <stdlib.h>
#include "waypoint_expansion.h"

/*  in meters */
#define DEFAULT_RAD 1

void read_file(FILE *fp, double rad)
{
    Waypoints wp = read_waypts(fp);
    expand_waypts(stdout, wp, rad);
}

int main(int argc, char *argv[]) {
    double rad = DEFAULT_RAD;
    if (argc == 1) {
        read_file(stdin, rad);
    } else if (argc <= 3) {
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: %s %s %s\n",
            argv[0], "Could not open file", 
            argv[1], "for reading");
            exit(1);
        }
        if (argc == 3) {
           rad = atof(argv[2]); 
        }
        read_file(fp, rad);
        fclose(fp);
    } else {
        fprintf(stderr, "%s: %s", argv[0],
        "run with too many arguments\n");
        exit(1);
    }
    return 0;
}
