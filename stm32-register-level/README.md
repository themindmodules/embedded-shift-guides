# STM32 register level projects

Six small programs for the NUCLEO-F411RE. No HAL, no CubeMX, no vendor library. Just a header with
register addresses, a start-up file and a linker script, so you can see everything that happens
between power on and your first line of `main()`.

> **Status:** all six projects build with `arm-none-eabi-gcc -Wall -Wextra -Werror` (see `build.sh`).
> Every register address and bit position in `common/stm32f411_regs.h` (65 values) was compared with
> ST's own CMSIS header, and the vector table of each build was checked (stack pointer, reset vector,
> SysTick and EXTI entries). The programs have **not** been run on a board yet.
> If something misbehaves on your board, please open an issue and tell me what you saw.

## Projects

| # | Folder | What it shows |
|---|---|---|
| 01 | `01-blink` | Enable a clock, set a pin as output, toggle it |
| 02 | `02-button-led` | Read an input pin, set and clear with BSRR |
| 03 | `03-systick-blink` | The core timer, an interrupt handler, 1 ms time base |
| 04 | `04-uart-hello` | Alternate function, baud rate maths, USART2 over the ST-LINK COM port |
| 05 | `05-pwm-breathing-led` | TIM2 PWM on the LED pin |
| 06 | `06-button-interrupt` | EXTI, SYSCFG, NVIC and sleeping with `wfi` |

## What is in `common/`

| File | Purpose |
|---|---|
| `stm32f411_regs.h` | Register addresses and bit masks used by the projects |
| `startup.c` | Vector table and `Reset_Handler` (copies `.data`, clears `.bss`, calls `main`) |
| `linker.ld` | Memory layout: 512 KB flash at `0x08000000`, 128 KB RAM at `0x20000000` |

## Build

You need `arm-none-eabi-gcc` on your PATH.

```sh
sh build.sh 01-blink       # one project
sh build.sh all            # all of them
```

Output goes to `build/`: an `.elf` for the debugger, a `.bin` file, and a map file.

## Flash

Use STM32CubeProgrammer with the ST-LINK: open `build/01-blink.bin`, set the start address to
`0x08000000`, connect and download. Or load the `.elf` in a debugger.

## The chip runs at 16 MHz

Nothing here sets up the PLL, so the chip runs on its internal 16 MHz oscillator (HSI). All the
timing numbers in the code (SysTick reload, UART baud rate, timer prescaler) assume 16 MHz. If you
change the clock, you must change those numbers too. That's a good exercise by itself.

## Exercises

Try these before you read the next project.

1. **06:** The button bounces, so one press can toggle the LED twice. Fix it with the SysTick
   millisecond counter: ignore a second press within 50 ms.
2. **03:** Change the blink to 5 Hz without touching the delay function. Which register do you change?
3. **04:** Add USART2 receive (`PA3`, `RE` bit, `RXNE` flag) and echo back every character.
4. **05:** Make the fade slower and give it a gamma curve, so it looks even to the eye.
5. **02:** Make the LED toggle on each press instead of following the button.
6. **Any:** Set the clock to 84 MHz using the PLL and fix every number that depended on 16 MHz.

## Where to read next

- `../guides/03-start-register-level.md`: the method behind these programs
- RM0383, the STM32F411 reference manual: the final source for every register
