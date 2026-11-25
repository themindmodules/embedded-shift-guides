# Starting register level programming

A register is just a memory location with a job. When you call `digitalWrite()` or `HAL_GPIO_WritePin()`,
some code finally writes a number into one of these locations. Register level programming means you
do that write yourself.

You don't need to do this for every project. You do it to understand what the chip is doing, and
for the moments when a library is too slow, too big, or wrong.

## Step 1: Pick one chip and one board

Don't try to learn "registers" in general. Pick one chip. This guide uses the **STM32F411
(NUCLEO-F411RE)**, and the code in `stm32-register-level/` runs on it. The same method works on any
microcontroller.

## Step 2: Get two documents

1. **Reference manual** (RM0383 for the F411). It describes every peripheral and register.
2. **Datasheet**. It tells you which pin can do which function and the electrical limits.

You'll read very small parts of these, never all of it. Learn to use the PDF search and the
bookmarks.

## Step 3: Understand the memory map

Everything is an address. The reference manual has a memory map table. For the F411:

| Block | Starts at |
|---|---|
| Flash | `0x0800 0000` |
| RAM | `0x2000 0000` |
| TIM2 | `0x4000 0000` |
| USART2 | `0x4000 4400` |
| GPIOA | `0x4002 0000` |
| RCC (clocks) | `0x4002 3800` |

A register is a base address plus an offset. For example, the output data register of port A is
`0x40020000 + 0x14`. In C:

```c
#define GPIOA_ODR (*(volatile uint32_t *)(0x40020000UL + 0x14UL))
GPIOA_ODR = 0x20;     /* writes 0x20 to that address */
```

`volatile` tells the compiler the hardware can change the value, so it must not remove or cache the
access.

## Step 4: The pattern for every peripheral

Almost every peripheral follows the same four steps:

1. **Enable its clock** (in RCC). Peripherals are switched off by default to save power.
2. **Set up the pins** (mode, alternate function, pull-up).
3. **Configure the peripheral** (speed, mode, limits).
4. **Enable it and use it.**

If something does nothing, check step 1 first. It's the most common reason.

## Step 5: Set, clear, toggle and test bits

You'll do these ten thousand times, so learn them now.

```c
REG |=  (1UL << n);        /* set bit n          */
REG &= ~(1UL << n);        /* clear bit n        */
REG ^=  (1UL << n);        /* toggle bit n       */
if (REG & (1UL << n)) {}   /* test bit n         */

/* a 2 bit field, for example pin 5 in MODER sits at bits 11:10 */
REG &= ~(3UL << 10);       /* clear the field first */
REG |=  (1UL << 10);       /* then write the new value */
```

Always clear a field before you set it. Writing `|=` alone can leave old bits behind.

## Step 6: Walk through blinking an LED

Goal: LED on PA5 (the Nucleo LED). Do the lookup yourself in the reference manual:

1. **Clock:** RCC_AHB1ENR, bit 0 is GPIOAEN. Set it.
2. **Mode:** GPIOA_MODER. PA5 uses bits 11:10. Value `01` means output.
3. **Toggle:** GPIOA_ODR bit 5.

```c
RCC_AHB1ENR |= (1UL << 0);
GPIOA_MODER &= ~(3UL << 10);
GPIOA_MODER |=  (1UL << 10);
for (;;) { GPIOA_ODR ^= (1UL << 5); /* wait a bit */ }
```

That's the whole program. `stm32-register-level/01-blink` is this with comments.

## Step 7: Check your work in the debugger

Open the **SFRs** (special function registers) view in CubeIDE while the program runs. You'll see
`RCC_AHB1ENR` bit 0 turn to 1 and the GPIOA registers change as you step through the code.
If the register shows what you wrote, your code is right and the problem is somewhere else.

## Step 8: Read the library to see what it does

Open a HAL function such as `HAL_GPIO_Init` and read it. It's long, but you'll recognise the
`MODER`, `OSPEEDR` and `PUPDR` writes. This is the fastest way to learn which registers matter.

## Step 9: Do the projects in order

| # | Project | What you learn |
|---|---|---|
| 01 | Blink | Clock enable, pin mode, ODR |
| 02 | Button and LED | Reading IDR, BSRR for set and clear |
| 03 | SysTick blink | The core timer, an interrupt handler, vector table |
| 04 | UART hello | Alternate functions, baud rate calculation |
| 05 | PWM LED | Timer prescaler, auto reload and compare |
| 06 | Button interrupt | EXTI, SYSCFG and the NVIC |

Do the exercises at the end of each README before you move on.

## Common mistakes

| Mistake | Fix |
|---|---|
| Forgot to enable the clock | Check RCC first |
| Set a 2 bit field with OR only, without clearing it first | Clear the field, then set it |
| Variable changed by an interrupt, but the loop never sees it | Declare it `volatile` |
| Handler never runs | Its name must match the vector table exactly |
| Wrong pin function | Look up the alternate function table in the datasheet |
| Interrupt keeps firing | Clear the pending flag inside the handler |
