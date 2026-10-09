#include <stdint.h>
#include "stm32f401.h"
#include "timer.h"
#include "uart.h"
#include "i2c.h"
#include "mpu6500.h"

int main(void) {
    //Enable clocks for both GPIOA and GPIOC
    RCC_AHB1ENR |= (1 << 0) | (1 << 2); // set bits 0 and 2

    // Configure PA5 as output (01)
    GPIOA_MODER &= ~(3 << 10); // clear bits 10, 11
    GPIOA_MODER |= (1 << 10);  // set to 01

    // Configure PC13 as input (00)
    GPIOC_MODER &= ~(3 << 26); // clear bits 26, 27

    //Configure PC13's pull-up (01)
    GPIOC_PUPDR &= ~(3 << 26); // clear bits 26, 27
    GPIOC_PUPDR |= (1 << 26);  // set to 01

    timer_init();
    uart_init();
    i2c_init();

    if (mpu_init() != 0) {
        uart_puts("MPU not found\r\n");
    }

    uint32_t start = millis();

    while(1) {
        int16_t ax, ay, az;

        if (mpu_read_accel(&ax, &ay, &az) == 0){
            uart_puts("x: ");
            uart_putint(ax);
            uart_puts(" y: ");
            uart_putint(ay);
            uart_puts(" z: ");
            uart_putint(az);
            uart_puts("\r\n");
        } else {
            uart_puts("read error\r\n");
        }

        while((millis() - start) < 100);
        start += 100;
    }
}