#include <stdint.h>
#include "i2c.h"
#include "mpu6500.h"

#define MPU_ADDR     0x68
#define WHO_AM_I     0x75
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

int mpu_init(void) {
    uint8_t id;

    // read WHO_AM_I into id
    // if read fails, return -1
    if((i2c_read_reg(MPU_ADDR, WHO_AM_I, &id)) != 0) {
        return -1;
    }

    // if id isn't 0x70, return -1
    if (id != 0x70) {
        return -1;
    }

    // write 0x01 into PWR_MGMT_1
    // if it fials, return -1
    if (i2c_write_reg(MPU_ADDR, PWR_MGMT_1, 0x01) != 0) {
        return -1;
    }

    return 0;
}

int mpu_read_accel(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t xh, xl, yh, yl, zh, zl;

    // read the six registers, 0x3B to 0x40, into those six variables
    // if any reads fail, return -1
    if (i2c_read_reg(MPU_ADDR, ACCEL_XOUT_H, &xh) != 0) {
        return -1;
    }
    if (i2c_read_reg(MPU_ADDR, ACCEL_XOUT_L, &xl) != 0) {
        return -1;
    }
    if (i2c_read_reg(MPU_ADDR, ACCEL_YOUT_H, &yh) != 0) {
        return -1;
    }
    if (i2c_read_reg(MPU_ADDR, ACCEL_YOUT_L, &yl) != 0) {
        return -1;
    }
    if (i2c_read_reg(MPU_ADDR, ACCEL_ZOUT_H, &zh) != 0) {
        return -1;
    }
    if (i2c_read_reg(MPU_ADDR, ACCEL_ZOUT_L, &zl) != 0) {
        return -1;
    }

    // combine each high/low pair into one 16-bit value
    *x = (int16_t)((xh << 8) | xl);
    *y = (int16_t)((yh << 8) | yl);
    *z = (int16_t)((zh << 8) | zl);

    return 0;
}