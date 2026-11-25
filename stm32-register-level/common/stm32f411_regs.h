/* Minimal register map for the STM32F411 (Nucleo-F411RE), written by hand for learning.
   Only the registers used by the projects in this folder are listed.
   Source: RM0383 (STM32F411 reference manual) and PM0214 (Cortex-M4 programming manual).
   Every address and bit position here was also compared against the ST CMSIS header
   with a compile-time check. */
#ifndef STM32F411_REGS_H
#define STM32F411_REGS_H

#include <stdint.h>

/* One 32 bit register at a fixed address. volatile tells the compiler that the hardware can change it. */
#define REG(addr) (*(volatile uint32_t *)(addr))

/* ---------- RCC: reset and clock control, base 0x40023800 ---------- */
#define RCC_BASE      0x40023800UL
#define RCC_CR        REG(RCC_BASE + 0x00UL)
#define RCC_CFGR      REG(RCC_BASE + 0x08UL)
#define RCC_AHB1ENR   REG(RCC_BASE + 0x30UL)
#define RCC_APB1ENR   REG(RCC_BASE + 0x40UL)
#define RCC_APB2ENR   REG(RCC_BASE + 0x44UL)

#define RCC_AHB1ENR_GPIOAEN  (1UL << 0)
#define RCC_AHB1ENR_GPIOBEN  (1UL << 1)
#define RCC_AHB1ENR_GPIOCEN  (1UL << 2)
#define RCC_APB1ENR_TIM2EN   (1UL << 0)
#define RCC_APB1ENR_USART2EN (1UL << 17)
#define RCC_APB2ENR_SYSCFGEN (1UL << 14)

/* ---------- GPIO ports ---------- */
#define GPIOA_BASE 0x40020000UL
#define GPIOB_BASE 0x40020400UL
#define GPIOC_BASE 0x40020800UL

#define GPIO_MODER(b)   REG((b) + 0x00UL)   /* 2 bits per pin: 00 input, 01 output, 10 alternate, 11 analog */
#define GPIO_OTYPER(b)  REG((b) + 0x04UL)   /* 1 bit per pin: 0 push-pull, 1 open drain */
#define GPIO_OSPEEDR(b) REG((b) + 0x08UL)
#define GPIO_PUPDR(b)   REG((b) + 0x0CUL)   /* 2 bits per pin: 00 none, 01 pull-up, 10 pull-down */
#define GPIO_IDR(b)     REG((b) + 0x10UL)   /* input data, read only */
#define GPIO_ODR(b)     REG((b) + 0x14UL)   /* output data */
#define GPIO_BSRR(b)    REG((b) + 0x18UL)   /* bits 0..15 set a pin, bits 16..31 clear a pin */
#define GPIO_AFRL(b)    REG((b) + 0x20UL)   /* alternate function, pins 0..7, 4 bits each  */
#define GPIO_AFRH(b)    REG((b) + 0x24UL)   /* alternate function, pins 8..15, 4 bits each */

#define GPIOA_MODER GPIO_MODER(GPIOA_BASE)
#define GPIOA_IDR   GPIO_IDR(GPIOA_BASE)
#define GPIOA_ODR   GPIO_ODR(GPIOA_BASE)
#define GPIOA_BSRR  GPIO_BSRR(GPIOA_BASE)
#define GPIOA_AFRL  GPIO_AFRL(GPIOA_BASE)
#define GPIOC_MODER GPIO_MODER(GPIOC_BASE)
#define GPIOC_PUPDR GPIO_PUPDR(GPIOC_BASE)
#define GPIOC_IDR   GPIO_IDR(GPIOC_BASE)

/* ---------- SysTick, part of the Cortex-M4 core, base 0xE000E010 ---------- */
#define STK_CTRL  REG(0xE000E010UL)
#define STK_LOAD  REG(0xE000E014UL)
#define STK_VAL   REG(0xE000E018UL)

#define STK_CTRL_ENABLE    (1UL << 0)
#define STK_CTRL_TICKINT   (1UL << 1)
#define STK_CTRL_CLKSOURCE (1UL << 2)   /* 1 = processor clock */
#define STK_CTRL_COUNTFLAG (1UL << 16)

/* ---------- USART2, base 0x40004400 (PA2 = TX, PA3 = RX, alternate function 7) ---------- */
#define USART2_BASE 0x40004400UL
#define USART2_SR   REG(USART2_BASE + 0x00UL)
#define USART2_DR   REG(USART2_BASE + 0x04UL)
#define USART2_BRR  REG(USART2_BASE + 0x08UL)
#define USART2_CR1  REG(USART2_BASE + 0x0CUL)

#define USART_SR_RXNE (1UL << 5)
#define USART_SR_TC   (1UL << 6)
#define USART_SR_TXE  (1UL << 7)
#define USART_CR1_RE  (1UL << 2)
#define USART_CR1_TE  (1UL << 3)
#define USART_CR1_UE  (1UL << 13)

/* ---------- TIM2, 32 bit general purpose timer, base 0x40000000 (PA5 = CH1, alternate function 1) ---------- */
#define TIM2_BASE  0x40000000UL
#define TIM2_CR1   REG(TIM2_BASE + 0x00UL)
#define TIM2_EGR   REG(TIM2_BASE + 0x14UL)
#define TIM2_CCMR1 REG(TIM2_BASE + 0x18UL)
#define TIM2_CCER  REG(TIM2_BASE + 0x20UL)
#define TIM2_CNT   REG(TIM2_BASE + 0x24UL)
#define TIM2_PSC   REG(TIM2_BASE + 0x28UL)
#define TIM2_ARR   REG(TIM2_BASE + 0x2CUL)
#define TIM2_CCR1  REG(TIM2_BASE + 0x34UL)

#define TIM_CR1_CEN        (1UL << 0)
#define TIM_CR1_ARPE       (1UL << 7)
#define TIM_EGR_UG         (1UL << 0)
#define TIM_CCMR1_OC1PE    (1UL << 3)
#define TIM_CCMR1_OC1M_PWM1 (6UL << 4)   /* output compare 1 mode = PWM mode 1 */
#define TIM_CCER_CC1E      (1UL << 0)

/* ---------- SYSCFG and EXTI: external pin interrupts ---------- */
#define SYSCFG_BASE   0x40013800UL
#define SYSCFG_EXTICR4 REG(SYSCFG_BASE + 0x14UL)   /* EXTI12..15 source port, 4 bits each */
#define EXTI_BASE     0x40013C00UL
#define EXTI_IMR      REG(EXTI_BASE + 0x00UL)
#define EXTI_RTSR     REG(EXTI_BASE + 0x08UL)
#define EXTI_FTSR     REG(EXTI_BASE + 0x0CUL)
#define EXTI_PR       REG(EXTI_BASE + 0x14UL)

/* ---------- NVIC: interrupt enable, part of the core ---------- */
#define NVIC_ISER1    REG(0xE000E104UL)            /* interrupts 32..63 */
#define EXTI15_10_IRQ 40u

#endif
