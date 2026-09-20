#include <stdint.h>

#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830)
#define GPIOA_MODER  (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR    (*(volatile uint32_t *)0x40020014)
#define GPIOC_MODER  (*(volatile uint32_t *)0x40020800)
#define GPIOC_PUPDR  (*(volatile uint32_t *)0x4002080C)
#define GPIOC_IDR    (*(volatile uint32_t *)0x40020810)

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

    while(1) {
        if (!(GPIOC_IDR & (1 << 13))) {
            GPIOA_ODR |= (1 << 5);
        } else {
            GPIOA_ODR &= ~(1 << 5);
        }
    }
}