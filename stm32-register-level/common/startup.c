/* Minimal start-up code for the STM32F411 (Cortex-M4), written in C.
   What happens at reset:
   1. The core reads the stack pointer from word 0 of the vector table and the
      reset vector from word 1, then jumps to Reset_Handler.
   2. Reset_Handler copies initialised globals from flash to RAM, clears the
      uninitialised ones, and calls main().
   No clock set-up is done, so the chip runs on its internal 16 MHz oscillator (HSI). */
#include <stdint.h>

extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;
extern int main(void);

void Default_Handler(void);
void Reset_Handler(void);

/* Every handler below can be replaced by simply defining a function with the same name. */
void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

#define N_VECTORS (16 + 86)   /* 16 core entries plus 86 interrupts on the F411 */

#pragma GCC diagnostic ignored "-Woverride-init"

__attribute__((section(".isr_vector"), used))
void (*const g_vectors[N_VECTORS])(void) = {
    [0 ... N_VECTORS - 1] = Default_Handler,   /* everything unused ends in a safe loop */
    [0]  = (void (*)(void))(&_estack),         /* initial stack pointer */
    [1]  = Reset_Handler,
    [2]  = NMI_Handler,
    [3]  = HardFault_Handler,
    [11] = SVC_Handler,
    [14] = PendSV_Handler,
    [15] = SysTick_Handler,
    [16 + 40] = EXTI15_10_IRQHandler,          /* interrupt number 40 */
};

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    for (uint32_t *dst = &_sdata; dst < &_edata; ) {
        *dst++ = *src++;
    }
    for (uint32_t *p = &_sbss; p < &_ebss; ) {
        *p++ = 0;
    }
    (void)main();
    for (;;) {
    }
}

void Default_Handler(void)
{
    for (;;) {
    }
}
