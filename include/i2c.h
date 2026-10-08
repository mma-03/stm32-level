#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c_init(void);

// start condition
void i2c_start(void);

// send address byte
int i2c_send_addr(uint8_t addr);

void i2c_stop(void);

#endif