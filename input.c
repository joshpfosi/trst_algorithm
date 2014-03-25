/*   File: input.c
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Mon Mar 24 23:52:36
 *
 *  file for reading in input as specified in state_rep.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "input.h"
#include "state_rep.h"

/* reads a data string into boat and enviroment data */
int update_state(char *data, Env_data env, Boat_data boat)
{
    /* read env */
    int re = sscanf(data, DATA_FORMAT_STRING, DATA_ARGS);

    /* Skip badly formed lines */
    if (re != NUM_MEMS) {
        fprintf(stderr, "Error reading data, read in: %d / %d necessary "
                "members\n", re, NUM_MEMS);
        return 1;
    }
    #ifdef DATA_GEN
        float boat_angle = (M_PI * boat->heading) / 180,
              boat_speed = 10, 
              wind_angle = (M_PI * env->wind_dir) / 180, 
              wind_speed = env->wind_speed;

        /* resolve boat vectors */
        float boat_x = boat_speed * cos(boat_angle), 
              boat_y = boat_speed * sin(boat_angle);

        /* resolve wind vectors */
        float wind_x = wind_speed * cos(wind_angle), 
              wind_y = wind_speed * sin(wind_angle);

        /* calculate vector product */
        float app_wind_x = boat_x + wind_x, app_wind_y = boat_y + wind_y;
        
        /* determine vector components */
        float temp = (180 * atan(app_wind_y / app_wind_x)) / M_PI;

        env->app_wind_dir = (temp < 0) ? temp + 360 : temp;
        env->app_wind_speed = sqrt(app_wind_x * app_wind_x + app_wind_y * app_wind_y);
    #endif

    return 0;
}

unsigned read_waypts(FILE *fp, Position *waypts, unsigned size) {
    char *line = malloc(16);
    unsigned num_waypoints = (fgetc(fp) - 48), i;
    assert(num_waypoints >= 0 && num_waypoints <= size);

    /* read in each waypt */
    for (i = 0; i < num_waypoints; ++i) {
        assert(fscanf(fp, "%f,%f;", &(waypts[i].lat), &(waypts[i].lon)) == 2);
    }
    fgetc(fp); /* discard the newline */

    free(line);
    return num_waypoints;
}

void output_state(FILE *fp, Env_data env, Boat_data boat) {
    fprintf(fp, OUTPUT_FORMAT, OUTPUT_ARGS);
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
