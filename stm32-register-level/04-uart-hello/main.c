/* 04 - Send text over USART2. On the Nucleo-F411RE this goes to the ST-LINK virtual COM port,
   so open a serial terminal at 115200 8N1 and read it on the PC.
   PA2 = USART2_TX, alternate function 7. */
#include "stm32f411_regs.h"

#define TX_PIN 2u

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

static void uart_putc(char c)
{
    while ((USART2_SR & USART_SR_TXE) == 0u) {   /* wait until the data register is empty */
    }
    USART2_DR = (uint32_t)(uint8_t)c;
}

static void uart_puts(const char *s)
{
    while (*s != '\0') {
        uart_putc(*s++);
    }
    while ((USART2_SR & USART_SR_TC) == 0u) {    /* wait until the last byte is fully sent */
    }
}

int main(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_APB1ENR |= RCC_APB1ENR_USART2EN;
    (void)RCC_APB1ENR;

    /* PA2 to alternate function mode (10), then pick function 7 in AFRL (pin 2 uses bits 11:8). */
    GPIOA_MODER &= ~(3UL << (TX_PIN * 2u));
    GPIOA_MODER |=  (2UL << (TX_PIN * 2u));
    GPIOA_AFRL  &= ~(0xFUL << (TX_PIN * 4u));
    GPIOA_AFRL  |=  (7UL   << (TX_PIN * 4u));

    /* Baud rate. USARTDIV = 16000000 / (16 x 115200) = 8.68. Mantissa 8, fraction 0.68 x 16 = 11 (0xB).
       The real baud rate is then 16000000 / (16 x 8.6875) = 115108, which is 0.1 percent off. */
    USART2_BRR = (8UL << 4) | 11UL;
    USART2_CR1 = USART_CR1_UE | USART_CR1_TE;

    STK_LOAD = 16000UL - 1UL;
    STK_VAL  = 0UL;
    STK_CTRL = STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE;

    for (;;) {
        uart_puts("hello from register level\r\n");
        delay_ms(1000u);
    }
}
