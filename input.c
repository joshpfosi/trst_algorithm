/*   File: input.c
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Sat Mar 22 21:33:17
 *
 *  file for reading in input as specified in state_rep.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "input.h"
#include "state_rep.h"

/* reads a data string into boat and enviroment data */
int update_state(char *data, Env_data env, Boat_data boat)
{
    #ifdef DATA_GEN
        (void)boat;
    #endif
    /* read env */
    int re = sscanf(data, DATA_FORMAT_STRING, DATA_ARGS);

    /* Skip badly formed lines */
    if (re != NUM_MEMS) {
        fprintf(stderr, "Error reading data, read in: %d / %d necessary "
                "members\n", re, NUM_MEMS);
        return 1;
    }
    return 0;
}

unsigned read_waypts(FILE *fp, Position *waypts, unsigned size) {
    char *line = malloc(16);
    unsigned num_waypoints = (fgetc(fp) - 48), i;
    assert(num_waypoints >= 0 && num_waypoints <= size);

    /* read in each waypt */
    for (i = 0; i < num_waypoints; ++i) {
        fscanf(fp, "%f,%f;", &(waypts[i].lat), &(waypts[i].lon));
    }
    fgetc(fp); /* discard the newline */

    free(line);
    return num_waypoints;
}

void print_nav(Navigator nav) {
    fprintf(stderr, "\n----- Printing Navigator -----\n");
    print_env(nav->env);
    print_boat(nav->boat);
    fprintf(stderr, "current_waypt            == %u\n"
                    "num_waypts               == %u\n",
                    nav->current_waypt, nav->num_waypts);
}

void print_env(Env_data env) {
    fprintf(stderr, "\n----- Printing Env_data  -----\n"
                    "env->wind_dir            == %f\n"
                    "env->wind_speed          == %f\n"
                    "env->app_wind_dir        == %f\n"
                    "env->app_wind_speed      == %f\n",
                    env->wind_dir, env->wind_speed, 
                    env->app_wind_dir, env->app_wind_speed);
}
void print_boat(Boat_data boat) {
    fprintf(stderr, "\n----- Printing Boat_data -----\n");
    print_position(boat->pos);
    fprintf(stderr, "boat->rud_pos            == %f\n"
                    "boat->sail_pos           == %f\n"
                    "boat->heading            == %f\n",
                    boat->rud_pos, boat->sail_pos,
                    boat->heading);
}

void print_position(Position pos) {
    fprintf(stderr, "\n----- Printing Position -----\n"
                    "pos.lat                 == %f\n"
                    "pos.lon                 == %f\n",
                    pos.lat, pos.lon);
}

FILE *open_or_abort(char *filename, char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        perror(filename);
        exit(1);
    }
    return fp;
}
