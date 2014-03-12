/*  input.h
 *  ALEX TONG C:11/3/14 M:11/3/14
 *
 *  header file for getting input data on robotic sailboat TRST project
 */

/* stores the data read in in *read in* order,
   use define statements in input_format.h to use data */
typedef struct env_dat {
    double dat[num_env_mems];
} *env_dat;

/* stores the data read in in *read in* order,
   use define statements in input_format.h to use data */
typedef struct boat_dat {
    double dat[num_boat_mems];
} *boat_dat;

/* reads a data string into enviroment data */
int input_env (char *data, env_dat env);

/* reads a data string into boat data */
int input_boat(char *data, boat_dat boat);
