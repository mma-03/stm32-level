#include <stdint.h>
#include "stm32f401.h"
#include "timer.h"

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

static volatile uint32_t ms_ticks = 0;

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

uint32_t millis(void) {
    return ms_ticks;
}

