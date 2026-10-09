#include <stdint.h>
#include <math.h>
#include "tilt.h"

#define ALPHA 0.1f

static float fx, fy, fz; // smoothed readings, kept between calls
static int first = 1;    // 1 until the first reading has been taken

void tilt_update(int16_t ax, int16_t ay, int16_t az, float *pitch, float *roll) {
    if (first == 1) {
        fx = (float)ax;
        fy = (float)ay;
        fz = (float)az;
        first = 0;
    } else {
        fx = fx + ALPHA * ((float)ax - fx);
        fy = fy + ALPHA * ((float)ay - fy);
        fz = fz + ALPHA * ((float)az - fz);
    }

    // calculate roll and pitch
    *roll  = atan2f(fy, fz) * 57.2958f;
    *pitch = atan2f(-fx, sqrtf(fy*fy + fz*fz)) * 57.2958f;
}


