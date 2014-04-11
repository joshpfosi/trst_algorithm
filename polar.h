
#include "state_rep.h"

/* returns an approximate boat speed from the given data
 * currently only takes into account app_wind_dir, and boat
 * heading */
float ideal_speed(Env_data env, Boat_data boat);
