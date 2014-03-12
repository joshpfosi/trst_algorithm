/*  main.c
 *  ALEX TONG C:11/3/14 M:11/3/14
 *
 *  main file for trst robotic sailboat project
 */

#include "input.h"

int main(int argc, char *argv[])
{
    env_dat  env  = malloc(sizeof(env));
    boat_dat boat = malloc(sizeof(boat));
    int exit_status = 0;

    /* reads in data until there is an error */
    while (!exit_status) {
        exit_status += input_env (/*TODO where does data come from?*/, env);
        exit_status += input_boat(/*TODO where does data come from?*/, boat);
    }
    return exit_status;
}