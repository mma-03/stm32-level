#include <stdint.h>
#include <math.h>
#include "tilt.h"

#define ALPHA 0.1f

static float fx, fy, fz; // smoothed readings, kept between calls
static int first = 1;    // 1 until the first reading has been taken
static float pitch_offset = 0.0f;
static float roll_offset = 0.0f;
static float last_pitch, last_roll; // most recent measured angles


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

    // calculate roll and pitch into local variables
    last_roll  = atan2f(fy, fz) * 57.2958f;
    last_pitch = atan2f(-fx, sqrtf(fy*fy + fz*fz)) * 57.2958f;

    // measured value - offset
    *roll = last_roll - roll_offset;
    *pitch = last_pitch - pitch_offset;
}

void tilt_zero(void) {
    // set pitch_offset to last_pitch
    pitch_offset = last_pitch;

    // set roll_offset to last_roll
    roll_offset = last_roll;
}

