/*   File: input_format.h
 *   By: Alex Tong, Date: Fri Mar 21
 *   Last Updated: Sat Mar 22 10:32:33
 *
 *  input formatting information for TRST robotic sailing project
 */

/*  information is kept in the order it was read in respective data structs
    additional define statements convert the data to the keyword/member name
    such that external modules with access to data can
        1. include input_format.h
        2. access data members by name
    this allows a separation between input and execution.
*/



/* ENV vars */
/* ex. #define AWS env->data[0];  ect. */


/* BOAT vars */
/* ex. #define SOG boat->dat[0];  ect. */


/* enviroment input defines */
#define num_mems           9
#define data_format_string "%f;%f;%f;%f;%f;%f;%f;%f;%f\n"
#define data_args          &(env->wind_dir), &(env->wind_speed),\
                           &(env->app_wind_dir), &(env->app_wind_speed),\
                           &(boat->rud_pos), &(boat->sail_pos),\
                           &(boat->pos.lat), &(boat->pos.lon), &(boat->heading)


