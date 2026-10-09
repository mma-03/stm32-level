#include <stdint.h>
#include <math.h>
#include "tilt.h"

void tilt_from_accel(int16_t ax, int16_t ay, int16_t az, float *pitch, float *roll) {
    //convert each reading to float
    float fx = (float)ax;
    float fy = (float)ay;
    float fz = (float)az;

    // calculate roll and pitch
    *roll  = atan2f(fy, fz) * 57.2958f;
    *pitch = atan2f(-fx, sqrtf(fy*fy + fz*fz)) * 57.2958f;

    
}
