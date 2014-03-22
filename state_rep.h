/*   File: input_format.h
 *   By: Alex Tong, Date: Fri Mar 21
 *   Last Updated: Sat Mar 22 10:43:42
 *
 *  input formatting information for TRST robotic sailing project
 */

#ifndef STATE_REP_H
#define STATE_REP_H

/* enviroment input defines */
#define NUM_MEMS           9
#define DATA_FORMAT_STRING "%f;%f;%f;%f;%f;%f;%f;%f;%f\n"
#define DATA_ARGS          &(env->wind_dir), &(env->wind_speed),\
                           &(env->app_wind_dir), &(env->app_wind_speed),\
                           &(boat->rud_pos), &(boat->sail_pos),\
                           &(boat->pos.lat), &(boat->pos.lon), &(boat->heading)

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

#endif /* STATE_REP_H */
