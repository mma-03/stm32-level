#include <stdint.h>
#include "stm32f401.h"
#include "uart.h"

void uart_init(void) {
    // enable USART2's clock
    RCC_APB1ENR |= (1 << 17);

    // enable GPIOA's clock
    RCC_AHB1ENR |= (1 << 0);

    // set PA2 to alternate function mode (10), bit 5 set, bit 4 clear
    GPIOA_MODER &= ~(3 << 4); // clear bits 4 and 5
    GPIOA_MODER |= (1 << 5);  // set bit 5, clear bit 4

    // Select AF07 in AFRL
    GPIOA_AFRL &= ~(0xF << 8); // clear bits  8-11
    GPIOA_AFRL |= (7 << 8); // set bits 8-11: 0111

    // write 139 into BRR
    USART2_BRR = 139;

    // set TE and UE in CR1
    USART2_CR1 |= (1 << 13) | (1 << 3);
}

void uart_putc(char c) {
    // spin until TXE (bit 7 of USART_SR) is set
    while(!(USART2_SR & (1 << 7)));

    // write c into USART2_DR
    USART2_DR = c;
}

void uart_puts(const char *s) {
    // walk string and send each character
    for (int i = 0; s[i] != '\0'; i++) {
        // send s[i]
        uart_putc(s[i]);
    }
}

void uart_putint(int32_t n) {
    // if n = 0, send '0' and return
    if (n == 0) {
        uart_putc('0');
        return;
    } else if (n < 0) {
        uart_putc('-');
        n = -n;
    }

    char buf[11]; // room for up to 10 digits
    int count = 0; //how many digits are stored so far

    //pull digits off the end and store them
    while (n > 0) {
        // store the character for the last digit in buf[count]
        buf[count] = n % 10 + '0';
        count++;
        // remove the last digit from n
        n = n/10;
    }

    // send the array back in reverse order
    for (int i = count - 1; i >= 0; i--) {
        //send buf[i]
        uart_putc(buf[i]);
    }
}


