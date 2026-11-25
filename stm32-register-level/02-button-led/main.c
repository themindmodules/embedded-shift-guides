/* 02 - Read a button, drive the LED. The LED follows the button.
   Nucleo-F411RE: user button B1 is on PC13 and reads 0 while pressed (there is an external pull-up).
   LED LD2 is on PA5. */
#include "stm32f411_regs.h"

#define LED_PIN 5u
#define BTN_PIN 13u

int main(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
    (void)RCC_AHB1ENR;

    /* PA5 output (01). */
    GPIOA_MODER &= ~(3UL << (LED_PIN * 2u));
    GPIOA_MODER |=  (1UL << (LED_PIN * 2u));

    /* PC13 input (00). It is 00 after reset, but writing it shows the intent. */
    GPIOC_MODER &= ~(3UL << (BTN_PIN * 2u));

    for (;;) {
        if ((GPIOC_IDR & (1UL << BTN_PIN)) == 0u) {
            GPIOA_BSRR = (1UL << LED_PIN);             /* button pressed: set the pin (bits 0..15 set) */
        } else {
            GPIOA_BSRR = (1UL << (LED_PIN + 16u));     /* released: clear the pin (bits 16..31 clear) */
        }
    }
}
