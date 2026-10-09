#ifndef MPU6500_H
#define MPU6500_H

#include <stdint.h>

int mpu_init(void);
int mpu_read_accel(int16_t *x, int16_t *y, int16_t *z);

#endif