/*   File: state_rep.h
 *   By: Alex Tong, Date: Fri Mar 21
 *   Last Updated: Mon Sep 15 11:19:31
 *
 *  Input formatting and general struct definitions
 */

#ifndef STATE_REP_H
#define STATE_REP_H

/* enviroment input defines */
/* if data is not being generated internally, read in all 10 state variables
 * otherwise, only read in the 4 environmental ones, as the boat state is 
 * calculated internally
 * 
 * further, define output formats as the new state must be output so it can be 
 * read in again
 */
#ifndef DATA_GEN
    #define NUM_MEMS           10
    #define DATA_FORMAT_STRING "%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;\n"
    #define DATA_ARGS          &(env->wind_dir), &(env->wind_speed),\
                           &(env->app_wind_dir), &(env->app_wind_speed),\
                           &(boat->rud_pos), &(boat->sail_pos),\
                           &(boat->pos.lat), &(boat->pos.lon), &(boat->heading), \
                           &(boat->boat_speed)
#else
    #define NUM_MEMS           4
    #define DATA_FORMAT_STRING "%lf;%lf;%lf;%lf;\n"
    #define DATA_ARGS          &(env->wind_dir), &(env->wind_speed),\
                               &(env->app_wind_dir), &(env->app_wind_speed)

    #define OUTPUT_FORMAT      "%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf\n"
    #define OUTPUT_ARGS        env->wind_dir, env->wind_speed,\
                               env->app_wind_dir, env->app_wind_speed,\
                               boat->rud_pos, boat->sail_pos,\
                               boat->pos.lat, boat->pos.lon, boat->heading, \
                               boat->boat_speed
#endif

#define MAX_WAYPTS         10
#define GROOVE             5    
#define CLOSE_HAULED_ANGLE 50
#define WAYPT_RADIUS       0.000010 
#define MIN_TRIM           0
#define MAX_TRIM           90
#define ERROR_HISTORY_CAP  1000 
#define EARTH_R            6371           /* radius of earth in km */
#define RATE               0.00010416666  /* 1 / 960 */

/* meanigful name */
typedef double Angle;

/* stores lat/lon info in convenient package */
typedef struct Position {
    double lat;
    double lon;
} Position;

/* stores pid integral control data */
typedef struct Rudder_PID_data {
  double   prev_errors[ERROR_HISTORY_CAP];
  double   integral;
  int     pos ; /* position of next array update */
} *Rudder_PID_data;

/* stores environmental data */
typedef struct Env_data {
    /* may replace w/ 2 vectors */
    double wind_dir, wind_speed, app_wind_dir, app_wind_speed;
} *Env_data;

/* stores boat related data */
typedef struct Boat_data {
    Angle rud_pos, sail_pos, heading;
    Position pos;
    Rudder_PID_data PID;
    double boat_speed;
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
