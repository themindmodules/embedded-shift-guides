/* 05 - Hardware PWM on the LED pin with TIM2. The LED slowly fades up and down.
   PA5 can be TIM2 channel 1 (alternate function 1), so the on-board LED works as a PWM output.
   Timer clock 16 MHz, prescaler 16 gives a 1 MHz count, ARR 1000 gives a 1 kHz PWM. */
#include "stm32f411_regs.h"

#define LED_PIN 5u

static volatile uint32_t g_ms;

void SysTick_Handler(void)
{
    g_ms++;
}

static void delay_ms(uint32_t ms)
{
    uint32_t start = g_ms;
    while ((uint32_t)(g_ms - start) < ms) {
    }
}

int main(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;
    (void)RCC_APB1ENR;

    /* PA5: alternate function mode (10), function 1 (pin 5 uses AFRL bits 23:20). */
    GPIOA_MODER &= ~(3UL << (LED_PIN * 2u));
    GPIOA_MODER |=  (2UL << (LED_PIN * 2u));
    GPIOA_AFRL  &= ~(0xFUL << (LED_PIN * 4u));
    GPIOA_AFRL  |=  (1UL   << (LED_PIN * 4u));

    TIM2_PSC  = 16UL - 1UL;                  /* 16 MHz / 16 = 1 MHz */
    TIM2_ARR  = 1000UL - 1UL;                /* 1000 counts = 1 ms = 1 kHz */
    TIM2_CCR1 = 0UL;                         /* duty: 0 to 999 out of 1000 */
    TIM2_CCMR1 = TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;
    TIM2_CCER  = TIM_CCER_CC1E;              /* connect channel 1 to the pin */
    TIM2_EGR   = TIM_EGR_UG;                 /* load the prescaler and reload values now */
    TIM2_CR1   = TIM_CR1_ARPE | TIM_CR1_CEN; /* start counting */

    STK_LOAD = 16000UL - 1UL;
    STK_VAL  = 0UL;
    STK_CTRL = STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE;

    uint32_t duty = 0u;
    int32_t step = 1;
    for (;;) {
        TIM2_CCR1 = duty;
        if (duty >= 999u) {
            step = -1;
        } else if (duty == 0u) {
            step = 1;
        }
        duty = (uint32_t)((int32_t)duty + step);
        delay_ms(2u);                        /* about 2 s for a full up and down cycle */
    }
}
