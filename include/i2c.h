#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c_init(void);

// start condition
void i2c_start(void);

// send address byte
int i2c_send_addr(uint8_t addr);

// write data byte
int i2c_write_byte(uint8_t data);

uint8_t i2c_read_byte(void);

int i2c_read_reg(uint8_t dev, uint8_t reg, uint8_t *value);

int i2c_write_reg(uint8_t dev, uint8_t reg, uint8_t value);

void i2c_stop(void);

#endif