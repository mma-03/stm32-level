#ifndef TILT_H
#define TILT_H

#include <stdint.h>

void tilt_update(int16_t ax, int16_t ay, int16_t az, float *pitch, float *roll);

#endif