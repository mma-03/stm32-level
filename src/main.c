#include <stdint.h>
#include "stm32f401.h"
#include "timer.h"
#include "uart.h"
#include "i2c.h"

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

    int result;
    uint8_t value = 0;

    i2c_start();
    result = i2c_send_addr(0x68 << 1); // address + write
    if (result == 0) {
        result = i2c_write_byte(0x75); // "I want register 0x75"
    }
    if (result == 0) {
        i2c_start(); // repeated start
        result = i2c_send_addr((0x68 << 1) | 1); // address + read
    }
    if (result == 0) {
        value = i2c_read_byte();
    }

    uart_puts("result: ");
    uart_putint(result);
    uart_puts(" WHO_AM_I: ");
    uart_putint(value);
    uart_puts("\r\n");

    uint32_t start = millis();

    while(1) {
       uart_puts("ticks: ");
       uart_putint(millis());
       uart_puts("\r\n");
       GPIOA_ODR ^= (1 << 5);

       while ((millis() - start) < 1000);
       start += 1000;
    }
}