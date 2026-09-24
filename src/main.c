#include <stdint.h>

#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830)
#define GPIOA_MODER  (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR    (*(volatile uint32_t *)0x40020014)
#define GPIOC_MODER  (*(volatile uint32_t *)0x40020800)
#define GPIOC_PUPDR  (*(volatile uint32_t *)0x4002080C)
#define GPIOC_IDR    (*(volatile uint32_t *)0x40020810)

#define RCC_APB1ENR  (*(volatile uint32_t *)0x40023840)
#define TIM2_CR1     (*(volatile uint32_t *)0x40000000)
#define TIM2_CNT     (*(volatile uint32_t *)0x40000024)
#define TIM2_PSC     (*(volatile uint32_t *)0x40000028)
#define TIM2_ARR     (*(volatile uint32_t *)0x4000002C)
#define TIM2_SR      (*(volatile uint32_t *)0x40000010)

#define TIM2_DIER    (*(volatile uint32_t *)0x4000000C)
#define NVIC_ISER0   (*(volatile uint32_t *)0xE000E100)

// status register, offset = 0x00, TXE = bit 7
#define USART2_SR    (*(volatile uint32_t *)0x40004400)
// data register, offset = 0x04
#define USART2_DR    (*(volatile uint32_t *)0x40004404)
// baud rate register, offset = 0x08
#define USART2_BRR   (*(volatile uint32_t *)0x40004408)
// control register 1, offset = 0X0C
// UE (USART Enable) = 13, TE (Transmitter Enable) = 3, RE (Receiver Enable) = 2
#define USART2_CR1   (*(volatile uint32_t *)0x4000440C)
// offset = 0x20, AF07 = 0111, pin 2 occupies bits 8 to 11
#define GPIOA_AFRL   (*(volatile uint32_t *)0x40020020)

void timer_init(void) {
    // enable TIM2 clock on APB1
    RCC_APB1ENR |= (1 << 0);

    // PSC so the counter ticks once per microsecond
    TIM2_PSC = 15;

    // ARR so it wraps every 1000 ticks
    TIM2_ARR = 999;

    // set UIE
    TIM2_DIER |= (1 << 0);

    // Set bit 28 in NVIC_ISER0
    NVIC_ISER0 |= (1 << 28);

    // enable the counter
    TIM2_CR1 |= (1 << 0);
}

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

volatile uint32_t ms_ticks = 0;

void TIM2_IRQHandler(void) {
    // clear the UIF flag
    TIM2_SR &= ~(1 << 0);
    // increment ms_ticks
    ms_ticks++;
}

void delay_ms(uint32_t ms) {
    // read the current tick count
    uint32_t start = ms_ticks;
    // wait until enough ticks have accumulated
    while ((ms_ticks - start) < ms);
}

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

    uint32_t start = ms_ticks;

    while(1) {
       uart_puts("ticks: ");
       uart_putint(ms_ticks);
       uart_puts("\r\n");
       GPIOA_ODR ^= (1 << 5);
       //delay_ms(1000);

       while ((ms_ticks - start) < 1000);
       start += 1000;
    }
}