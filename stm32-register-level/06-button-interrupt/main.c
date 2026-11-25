/* 06 - Button interrupt (EXTI). Press the button, the LED toggles inside an interrupt handler.
   The main loop does nothing but sleep, which shows the CPU does not need to poll.
   Nucleo-F411RE: button B1 on PC13 (reads 0 when pressed), LED LD2 on PA5.
   Contact bounce can toggle the LED twice for one press. Fixing that is exercise 1 in the README. */
#include "stm32f411_regs.h"

#define LED_PIN 5u
#define BTN_PIN 13u

/* Interrupt 40 covers EXTI lines 10 to 15. The vector table in startup.c points at this name. */
void EXTI15_10_IRQHandler(void)
{
    if ((EXTI_PR & (1UL << BTN_PIN)) != 0u) {
        EXTI_PR = (1UL << BTN_PIN);             /* write 1 to clear the pending flag, or it fires again */
        GPIOA_ODR ^= (1UL << LED_PIN);
    }
}

int main(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
    RCC_APB2ENR |= RCC_APB2ENR_SYSCFGEN;        /* SYSCFG routes pins to EXTI lines and needs its clock */
    (void)RCC_APB2ENR;

    GPIOA_MODER &= ~(3UL << (LED_PIN * 2u));
    GPIOA_MODER |=  (1UL << (LED_PIN * 2u));
    GPIOC_MODER &= ~(3UL << (BTN_PIN * 2u));

    /* EXTI13 takes its pin from port C: field 7:4 of EXTICR4, value 2 means port C. */
    SYSCFG_EXTICR4 &= ~(0xFUL << 4);
    SYSCFG_EXTICR4 |=  (2UL   << 4);

    EXTI_FTSR |= (1UL << BTN_PIN);              /* trigger on the falling edge (button press pulls the pin low) */
    EXTI_IMR  |= (1UL << BTN_PIN);              /* unmask line 13 */
    NVIC_ISER1 = (1UL << (EXTI15_10_IRQ - 32u)); /* enable interrupt 40 in the core */

    for (;;) {
        __asm volatile ("wfi");                 /* sleep until the next interrupt */
    }
}
