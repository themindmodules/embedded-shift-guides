/* 03 - Exact 1 ms time base with SysTick, then blink at 2 Hz.
   After reset the core runs at 16 MHz (HSI), so 16000 ticks make 1 ms. */
#include "stm32f411_regs.h"

#define LED_PIN 5u

static volatile uint32_t g_ms;      /* volatile: changed inside the interrupt, read in main */

/* The vector table in startup.c points at this name. The core calls it every millisecond. */
void SysTick_Handler(void)
{
    g_ms++;
}

static void delay_ms(uint32_t ms)
{
    uint32_t start = g_ms;
    while ((uint32_t)(g_ms - start) < ms) {     /* the subtraction stays correct when g_ms wraps */
    }
}

int main(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    (void)RCC_AHB1ENR;
    GPIOA_MODER &= ~(3UL << (LED_PIN * 2u));
    GPIOA_MODER |=  (1UL << (LED_PIN * 2u));

    STK_LOAD = 16000UL - 1UL;                   /* counts 16000 clocks: reload value is N - 1 */
    STK_VAL  = 0UL;                             /* any write clears the counter */
    STK_CTRL = STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE;

    for (;;) {
        GPIOA_ODR ^= (1UL << LED_PIN);
        delay_ms(250u);                         /* 250 ms on, 250 ms off */
    }
}
