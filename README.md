# Embedded shift guides

Short step by step guides for moving from one level of embedded work to the next, plus small
register level STM32 projects to practice on. Made for the followers of @themindmodules.

> **Status:** the guides are written from the vendor datasheets and manuals, and I haven't
> proof-tested every step on every board. If a step doesn't work for you, tell me which board and
> which version you used. The STM32 projects build cleanly and their registers were checked
> against ST's header, but they haven't been run on a board yet. See the README inside that folder.

## Guides

| # | Guide | Read it when |
|---|---|---|
| 1 | [Arduino to ESP32](guides/01-arduino-to-esp32.md) | You know Arduino and want Wi-Fi, Bluetooth and two cores |
| 2 | [ESP32 to STM32](guides/02-esp32-to-stm32.md) | You need exact timing, low power or industrial style work |
| 3 | [Starting register level programming](guides/03-start-register-level.md) | You want to know what the library really does |
| 4 | [Resources](guides/04-resources.md) | You want manuals, books, courses and tools worth your time |
| 5 | [Tips and tricks](guides/05-tips-and-tricks.md) | You are stuck, or want to save time |

## Projects

| Folder | Contents |
|---|---|
| [stm32-register-level](stm32-register-level) | Six NUCLEO-F411RE programs written with direct register access: blink, button, SysTick, UART, PWM, button interrupt |

## Suggested order

Guide 1, then guide 2, then guide 3 together with the projects. Keep guide 4 and guide 5 open
while you work.

## License

Personal and educational use. You may use these files in your own projects. Please don't resell
or republish them.
