/* put your header here josh */

#include "polar.h"

float angle_between_angles(float f1, float f2)
{
        return f1 - f2;
}

/* J24 polar data from some random guy on the internet */
/*from: http://76trombones.wordpress.com/offshore-handicap-fleet/j24/ */
/*TODO boat needs a speed history, angle_between wind and boat might want to be
  a stored variable?
  how do we have wind_dir and wind_speed/why?
  */
float ideal_speed(Env_data env, Boat_data boat)
{
    /*TWA 30-180, TWS 6, 8, 10, 12, 14, 16, 20*/
    float polar_data[][7] =
    {   { 2.80,   3.53,   3.99,   4.28,   4.51,   4.69,   4.87 },
        { 3.25,   4.06,   4.55,   4.86,   5.11,   5.29,   5.47 },
        { 3.68,   4.53,   5.04,   5.36,   5.60,   5.76,   5.92 },
        { 4.09,   4.94,   5.42,   5.74,   5.94,   6.07,   6.20 },
        { 4.44,   5.23,   5.71,   6.00,   6.16,   6.27,   6.40 },
        { 4.71,   5.43,   5.90,   6.18,   6.33,   6.44,   6.58 },
        { 4.90,   5.58,   6.03,   6.29,   6.49,   6.63,   6.80 },
        { 5.03,   5.69,   6.12,   6.39,   6.63,   6.80,   7.00 },
        { 5.11,   5.77,   6.19,   6.49,   6.74,   6.95,   7.17 },
        { 5.16,   5.81,   6.24,   6.57,   6.83,   7.09,   7.33 },
        { 5.20,   5.85,   6.28,   6.64,   6.91,   7.22,   7.48 },
        { 5.22,   5.88,   6.30,   6.69,   6.98,   7.35,   7.62 },
        { 5.24,   5.91,   6.32,   6.72,   7.05,   7.48,   7.75 },
        { 5.24,   5.93,   6.32,   6.73,   7.10,   7.58,   7.88 },
        { 5.22,   5.91,   6.31,   6.72,   7.12,   7.63,   8.05 },
        { 5.16,   5.88,   6.29,   6.69,   7.11,   7.63,   8.23 },
        { 5.08,   5.81,   6.25,   6.64,   7.07,   7.58,   8.45 },
        { 4.97,   5.72,   6.20,   6.57,   7.00,   7.48,   8.66 },
        { 4.82,   5.61,   6.13,   6.49,   6.91,   7.36,   8.69 },
        { 4.64,   5.49,   6.05,   6.41,   6.80,   7.23,   8.50 },
        { 4.43,   5.35,   5.94,   6.32,   6.69,   7.10,   8.19 },
        { 4.20,   5.18,   5.80,   6.22,   6.57,   6.96,   7.90 },
        { 3.95,   4.97,   5.64,   6.11,   6.45,   6.82,   7.67 },
        { 3.71,   4.75,   5.47,   5.98,   6.34,   6.69,   7.48 },
        { 3.51,   4.52,   5.29,   5.85,   6.23,   6.56,   7.33 },
        { 3.30,   4.29,   5.11,   5.71,   6.13,   6.46,   7.20 },
        { 3.08,   4.07,   4.95,   5.57,   6.03,   6.36,   7.08 },
        { 2.88,   3.86,   4.78,   5.44,   5.94,   6.28,   6.97 },
        { 2.71,   3.69,   4.62,   5.33,   5.86,   6.22,   6.89 },
        { 2.57,   3.57,   4.48,   5.20,   5.78,   6.17,   6.82 },
        { 2.50,   3.50,   4.40,   5.10,   5.70,   6.14,   6.79 } };
    float angle_to_wind = angle_between_angles(env->app_wind_dir, boat->heading);
    if (angle_to_wind > 180)
            angle_to_wind = 360 - angle_to_wind;
    if (angle_to_wind < 30)
        return 0;
    int wa = angle_to_wind;
    wa -= 30;
    wa /= 5;
    /*now have a binned wind angle that can be used in polar data */
    float env_wind_speed = env->wind_speed;
    if (env_wind_speed < 6)
        env_wind_speed = 6;
    if (env_wind_speed > 16)
        /*no data for 18 */
        return polar_data[wa][6];
    int ws = env_wind_speed;
    ws -= 6;
    ws /= 2;
    /*now have a binned wind speed*/

    return polar_data[wa][ws];

}
