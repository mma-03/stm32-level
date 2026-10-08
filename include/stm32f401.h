#ifndef STM32F401_H
#define STM32F401_H

#include <stdint.h>

#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830)
#define GPIOA_MODER  (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR    (*(volatile uint32_t *)0x40020014)
#define GPIOC_MODER  (*(volatile uint32_t *)0x40020800)
#define GPIOC_PUPDR  (*(volatile uint32_t *)0x4002080C)
#define GPIOC_IDR    (*(volatile uint32_t *)0x40020810)

// timer + interrupts
#define RCC_APB1ENR  (*(volatile uint32_t *)0x40023840)
#define TIM2_CR1     (*(volatile uint32_t *)0x40000000)
#define TIM2_CNT     (*(volatile uint32_t *)0x40000024)
#define TIM2_PSC     (*(volatile uint32_t *)0x40000028)
#define TIM2_ARR     (*(volatile uint32_t *)0x4000002C)
#define TIM2_SR      (*(volatile uint32_t *)0x40000010)

#define TIM2_DIER    (*(volatile uint32_t *)0x4000000C)
#define NVIC_ISER0   (*(volatile uint32_t *)0xE000E100)

// UART
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

// I2C
// RM0368, GPIO registers
#define GPIOB_MODER  (*(volatile uint32_t *)0x40020400)
#define GPIOB_OTYPER (*(volatile uint32_t *)0x40020404)
#define GPIOB_AFRH   (*(volatile uint32_t *)0x40020424)
// RM0368, I2C register map
#define I2C1_CR1     (*(volatile uint32_t *)0x40005400)
#define I2C1_CR2     (*(volatile uint32_t *)0x40005404)
#define I2C1_CCR     (*(volatile uint32_t *)0x4000541C)
#define I2C1_TRISE   (*(volatile uint32_t *)0x40005420)
#define I2C1_DR      (*(volatile uint32_t *)0x40005410)
#define I2C1_SR1     (*(volatile uint32_t *)0x40005414)
#define I2C1_SR2     (*(volatile uint32_t *)0x40005418)

#endif