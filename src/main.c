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

void timer_init(void) {
    // enable TIM2 clock on APB1
    RCC_APB1ENR |= (1 << 0);

    // PSC so the counter ticks once per microsecond
    TIM2_PSC = 15;

    // ARR so it wraps every 1000 ticks
    TIM2_ARR = 999;

    // enable the counter
    TIM2_CR1 |= (1 << 0);
}

void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        // wait until UIF is set (counter wrapped)
        while (!(TIM2_SR & (1 << 0)));
        // clear UIF
        TIM2_SR &= ~(1 << 0);
    }
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

    while(1) {
       GPIOA_ODR ^= (1 << 5);
       delay_ms(500);
    }
}