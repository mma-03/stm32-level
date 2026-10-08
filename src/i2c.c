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