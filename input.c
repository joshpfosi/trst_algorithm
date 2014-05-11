/*   File: input.c
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Sat May 10 21:58:55
 *
 *  file for reading in input as specified in state_rep.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "state_rep.h"
#include "input.h"
#include "polar.h"
#include "helper.h"


static void   update_pos(Boat_data boat, double boat_speed);
extern double ideal_speed(Env_data env, Boat_data boat);

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
    double boat_angle = (M_PI * boat->heading) / 180,
          /*              boat_speed = 10, */
          boat_speed = ideal_speed(env, boat), 
          wind_angle = (M_PI * env->wind_dir) / 180, 
          /*              wind_angle = degrees_to_radians(env->wind_dir),*/
          wind_speed = env->wind_speed;
    //fprintf(stdout, "boat speed == %f\n", boat_speed); /* debugging */



    update_pos(boat, boat_speed);
    boat->boat_speed = boat_speed;

    /* resolve boat vectors */
    double boat_x = boat->boat_speed * cosf(boat_angle),
          boat_y = boat->boat_speed * sinf(boat_angle);

    /* resolve wind vectors */
    double wind_x = wind_speed * cosf(wind_angle), 
          wind_y = wind_speed * sinf(wind_angle);


    /* calculate vector product */
    double app_wind_x = boat_x + wind_x, app_wind_y = boat_y + wind_y;

    /* determine vector components */
    double temp = (180 * atan(app_wind_y / app_wind_x)) / M_PI;

    env->app_wind_dir = (temp < 0) ? temp + 360 : temp;
    env->app_wind_speed = sqrt(app_wind_x * app_wind_x + app_wind_y * app_wind_y);
#endif

    return 0;
}

static void update_pos(Boat_data boat, double boat_speed)
{
    /* convert velocity vectors to angular velocity */
    double boat_x = boat_speed * cosf(degrees_to_radians(boat->heading)),
          boat_y = boat_speed * sinf(degrees_to_radians(boat->heading));

    /* convert tangential velocity vectors to angular velocity */
    double ang_vel_y = boat_y / EARTH_R;
    double ang_vel_x = boat_x / EARTH_R;

    /* convert position to radians */
    double long_rad = degrees_to_radians(boat->pos.lon);
    double lat_rad  = degrees_to_radians(boat->pos.lat);

    /* add angular velocity times time to current position
     * and update boat's position and convert back
     * NOTE: f = 1 / t, so t = 1 / f, hence division
     */
    boat->pos.lat = radians_to_degrees(lat_rad + ang_vel_y * RATE);
    boat->pos.lon = radians_to_degrees(long_rad  + ang_vel_x * RATE);
}


unsigned read_waypts(FILE *fp, Position *waypts, unsigned size) {
    char *line = malloc(16); /* TODO 16 I believe is arbitrary and needs to change */
    unsigned num_waypoints = (fgetc(fp) - 48), i;
    assert(num_waypoints >= 0 && num_waypoints <= size);

    /* read in each waypt */
    for (i = 0; i < num_waypoints; ++i) {
        assert(fscanf(fp, "%lf,%lf;", &(waypts[i].lat), &(waypts[i].lon)) == 2);
    }
    fgetc(fp); /* discard the newline */

    free(line);
    return num_waypoints;
}

void output_state(FILE *fp, Env_data env, Boat_data boat) {
    fprintf(fp, OUTPUT_FORMAT, OUTPUT_ARGS);
}

