/*   File: update_state_test.c
 *   By: Joshua Pfosi, Date: Sat Mar 22
 *   Last Updated: Sat Mar 22 10:32:42
 *
 *   Test suite for update_state
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* enviroment input defines */
#define num_mems           9
#define data_format_string "%f;%f;%f;%f;%f;%f;%f;%f;%f\n"
#define data_args          &(env->wind_dir), &(env->wind_speed),\
                           &(env->app_wind_dir), &(env->app_wind_speed),\
                           &(boat->rud_pos), &(boat->sail_pos),\
                           &(boat->pos.lat), &(boat->pos.lon), &(boat->heading)

#define MAX_WAYPTS 10

/* stores lat/lon info in convenient package */
typedef struct Position {
    float lat;
    float lon;
} Position;

/* stores the data read in in *read in* order,
   use define statements in input_format.h to use data */
typedef struct Env_data {
    float wind_dir, wind_speed, app_wind_dir, app_wind_speed;
} *Env_data;

/* stores the data read in in *read in* order,
   use define statements in input_format.h to use data */
typedef struct Boat_data {
    float rud_pos, sail_pos, heading;
    Position pos;
} *Boat_data;

/* bundles complete state of navigator */
typedef struct Navigator {
    Env_data env;
    Boat_data boat;
    Position *waypts;
    unsigned current_waypt;
    unsigned num_waypts;
} *Navigator;

/* reads a data string into boat and enviroment data */
int update_state(char *data, Env_data env, Boat_data boat)
{
    /* read env */
    int re = sscanf(data, data_format_string, data_args);

    /* Skip badly formed lines */
    if (re != num_mems) {
        fprintf(stderr, "Error reading data, read in: %d / %d necessary "
                "members\n", re, num_mems);
        return 1;
    }

    return 0;
}

int main() {
    /* holds entire state of navigator - passed to pilot */
    Navigator nav = malloc(sizeof(*nav));
    nav->env = malloc(sizeof(*(nav->env)));
    /* note: may need to initialize the Position inner struct */
    nav->boat = malloc(sizeof(*(nav->boat)));
    nav->waypts = malloc(MAX_WAYPTS * sizeof(*(nav->waypts)));
    nav->current_waypt = 0;
    nav->num_waypts = 0;
    Env_data env = nav->env;
    Boat_data boat = nav->boat;

    char *line = NULL;
    size_t len = 0;

    /* reads in env and boat data */
    /* may replace getline with fgets in future */
    while (getline(&line, &len, stdin) != -1) {
        fprintf(stdout, "Read: %s\n", line);
        fprintf(stdout, "Updating states...\n");
        update_state(line, nav->env, nav->boat);
        fprintf(stdout, "env->wind_dir            = %f\n"
                        "env->wind_speed          = %f\n"
                        "env->app_wind_dir        = %f\n"
                        "env->app_wind_speed      = %f\n"
                        "boat->rud_pos            = %f\n"
                        "boat->sail_pos           = %f\n"
                        "boat->pos.lat            = %f\n"
                        "boat->pos.lon            = %f\n"
                        "boat->heading            = %f\n",
                        env->wind_dir, 
                        env->wind_speed,
                        env->app_wind_dir,
                        env->app_wind_speed,
                        boat->rud_pos,
                        boat->sail_pos,
                        boat->pos.lat,
                        boat->pos.lon,
                        boat->heading);
    }
    
    return 0;
}
