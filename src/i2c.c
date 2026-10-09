#include <stdint.h>
#include "stm32f401.h"
#include "i2c.h"

void i2c_init(void) {
    // clocks
    RCC_AHB1ENR |= (1 << 1); // GPIOB EN
    RCC_APB1ENR |= (1 << 21); // I2C1 EN

    // clear pins 8 and 9 
    GPIOB_MODER &= ~((3 << 16) | (3 << 18));
    // set pins 8 and 9 to alternate function mode (10)
    GPIOB_MODER |= ((1 << 17) | (1 << 19));

    // set pins 8 and 9 to open-drain
    GPIOB_OTYPER |= ((1 << 8) | (1 << 9)); 

    // clear pin 8 (3-0) and pin 9 (4-7)
    GPIOB_AFRH &= ~((15 << 0) | (15 << 4));
    // choose AF4 (0100)
    GPIOB_AFRH |= ((4 << 0) | (4 << 4));

    // settings
    I2C1_CR2 = 16;
    I2C1_CCR = 80;
    I2C1_TRISE = 17;

    // enable
    I2C1_CR1 |= (1 << 0);
}

void i2c_start(void) {
    // set the start bit in CR1
    I2C1_CR1 |= (1 << 8);
    
    // wait until SB is set in SR1
    while(!(I2C1_SR1 & (1 << 0)));
}

int i2c_send_addr(uint8_t addr) {
    // write address into DR
    I2C1_DR = addr;

    // waits for ADDR  or AF to be set
    while(!(I2C1_SR1 & ((1 << 1) | (1 << 10))));

    // if AF is set (sensor didn't answer)
    if (I2C1_SR1 & (1 << 10)) {
        // clear AF
        I2C1_SR1 &= ~(1 << 10);

        // send a STOP
        i2c_stop();

        return -1;

    } else {
        // clears ADDR by reading SR2
        (void)I2C1_SR2;

        return 0;
    }
}

int i2c_write_byte(uint8_t data) {
    // write data into DR
    I2C1_DR = data;

    // wait until BTF or AF is set
    while(!(I2C1_SR1 & ((1 << 2) | (1 << 10))));
    // if AF is set:
    if (I2C1_SR1 & (1 << 10)) {
        // clear AF
        I2C1_SR1 &= ~(1 << 10);

        // send a STOP
        i2c_stop();

        return -1;
    } else {
        return 0;
    }
}

uint8_t i2c_read_byte(void) {
    // call stop right adter ADDR is cleared 
    i2c_stop();

    // wait for RxNe
    while(!(I2C1_SR1 & (1 << 6)));

    //return DR
    return I2C1_DR;
}

int i2c_read_reg(uint8_t dev, uint8_t reg, uint8_t *value) {
    // start
    i2c_start();

    // send the addres with the write bit
    // if it returns -1, return -1
    if (i2c_send_addr(dev << 1) != 0) {
        return -1;
    }
    
    // send the register number, reg
    if (i2c_write_byte(reg) != 0) {
        return -1;
    }

    // send a repeated START
    i2c_start();

    // send the address with the read bit
    if (i2c_send_addr((dev << 1) | 1) != 0) {
        return -1;
    }

    // read the byte, and store it where value points
    *value = i2c_read_byte();

    return 0;
}

int i2c_write_reg(uint8_t dev, uint8_t reg, uint8_t value){
    // send a START
    i2c_start();

    // send the address with the WRITE bit
    if (i2c_send_addr(dev << 1) != 0) {
        return -1;
    }

    // send the register number, reg
    if(i2c_write_byte(reg) != 0) {
        return -1;
    }

    // send the value
    if(i2c_write_byte(value) != 0) {
        return -1;
    }

    i2c_stop();

    return 0;
}


void i2c_stop(void) {
    I2C1_CR1 |= (1 << 9);
}
