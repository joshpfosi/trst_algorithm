#include <stdio.h>
#include "state_rep.h"
#include "polar.h"
//compile with gcc -o polar_test polar_test.c polar.c

int main()
{
    struct Env_data env = {0, 0, 0, 0};
    struct Boat_data boat = {0, 0, 0, {0, 0}, 0};
    int i, j;
    env.app_wind_dir = 0;
    env.wind_dir = env.app_wind_dir;

    for(i = 4; i < 22; i++) {
        env.app_wind_speed = i;
        env.wind_speed = i;
        for (j = 0; j < 180; j+=5) {
            boat.heading = j;
            printf("ws: %f h: %f s: %f\n", env.app_wind_speed, boat.heading, ideal_speed(&env, &boat));
            //printf("%f ", ideal_speed(&env, &boat));
        }
        printf("\n");
    }
    return 0;
}