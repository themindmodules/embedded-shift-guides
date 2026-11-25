/* 01 - Blink the on-board LED (LD2 on PA5) with direct register writes.
   Board: NUCLEO-F411RE. The chip runs on its 16 MHz internal oscillator after reset. */
#include "stm32f411_regs.h"

#define LED_PIN 5u

/* Crude delay. The time depends on the compiler, so it is only roughly half a second.
   Project 03 replaces this with an exact SysTick timer. */
static void delay(volatile uint32_t count)
{
    while (count--) {
    }
}

int main(void)
{
    /* Step 1: peripherals are off after reset. Switch on the clock of GPIO port A. */
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    (void)RCC_AHB1ENR;                 /* read back so the clock is surely running before we touch GPIOA */

    /* Step 2: make PA5 an output. MODER has 2 bits per pin, so PA5 sits in bits 11:10. Value 01 = output. */
    GPIOA_MODER &= ~(3UL << (LED_PIN * 2u));   /* clear both bits */
    GPIOA_MODER |=  (1UL << (LED_PIN * 2u));   /* set to 01 */

    /* Step 3: toggle the pin forever. XOR flips the bit. */
    for (;;) {
        GPIOA_ODR ^= (1UL << LED_PIN);
        delay(1000000UL);
    }
}
