/*  input_format.h
 *  ALEX TONG C:11/3/14 M:11/3/14
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
#define num_env_mems        4
#define env_format_string   "%f %f %f %f\n"
#define env_args            env->dat[0], env->dat[1], env->dat[2], env->dat[3]

/* boat data input defines */
#define num_boat_mems       4
#define boat_format_string  "%f %f %f %f\n"
#define boat_args           boat->dat[0], boat->dat[1], boat->dat[2], boat->dat[3]

