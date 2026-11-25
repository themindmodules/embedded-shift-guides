# Resources for moving up and going low level

A short list. I kept it to things that are actually worth your time. Websites and course pages
change, so search the exact title if a link has moved.

## Official documents (free, and always the final answer)

| Document | Where | Why you need it |
|---|---|---|
| **RM0383**, STM32F411 reference manual | st.com | Every register of the F411. The main book for register level work |
| **STM32F411xC/E datasheet** | st.com | Pins, alternate functions, electrical limits |
| **PM0214**, STM32 Cortex-M4 programming manual | st.com | The core itself: SysTick, NVIC, fault handling |
| **UM1724**, STM32 Nucleo-64 user manual | st.com | Which pin is the LED, the button, the virtual COM port |
| **Cortex-M4 Devices Generic User Guide** | developer.arm.com | Instructions and core peripherals from ARM |
| **ESP32 Technical Reference Manual** | espressif.com | Registers of the ESP32 |
| **ESP-IDF Programming Guide** | docs.espressif.com | Espressif's own framework, the next step after Arduino |
| **Arduino-ESP32 documentation** | docs.espressif.com | API of the Arduino core, including the 3.x changes |

Tip: in a reference manual, use the search for the register name (for example `AHB1ENR`) and read
only that section and the one before it.

## Books

| Book | Good for |
|---|---|
| *The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors*, Joseph Yiu | How the core really works: interrupts, faults, the startup sequence |
| *Mastering STM32*, Carmine Noviello | A full walk through the STM32 world with CubeIDE |
| *Making Embedded Systems*, Elecia White | Design thinking: how to structure firmware that does not fall apart |
| *Embedded Systems with ARM Cortex-M Microcontrollers in Assembly Language and C*, Yifeng Zhu | Learning from the metal up |

Check which edition matches your chip and tools before buying.

## Video series and blogs

| Name | What it is |
|---|---|
| FastBit Embedded Brain Academy, "Mastering Microcontroller and Embedded Driver Development" | A paid course where you write the drivers at register level on STM32 |
| ControllersTech | STM32 HAL and peripheral tutorials, good for getting started fast |
| Phil's Lab | STM32 projects together with PCB design |
| Shawn Hymel, "Introduction to RTOS" | FreeRTOS explained step by step, with ESP32 examples |
| Embedded Artistry and the Interrupt blog (Memfault) | Articles on firmware design, debugging and tools |

## Tools you'll use

| Tool | For |
|---|---|
| STM32CubeIDE and CubeMX | Writing, building and debugging STM32 projects |
| STM32CubeProgrammer | Flashing files, option bytes, the built-in bootloader |
| VS Code with PlatformIO | A lighter editor setup, works for STM32 and ESP32 |
| A cheap 8 channel logic analyser with PulseView or Sigrok | Seeing UART, I2C and SPI traffic. Buy one early |
| A multimeter, and later an oscilloscope | Checking supply rails, signals and timing |
| KiCad | Your own boards, when you outgrow the dev board |

## A learning order that works

1. Arduino to ESP32: `01-arduino-to-esp32.md`
2. ESP32 to STM32 with HAL: `02-esp32-to-stm32.md`
3. Registers: `03-start-register-level.md` and the projects in `stm32-register-level/`
4. RTOS basics (FreeRTOS)
5. Your own PCB with an STM32 on it

Don't read all of this first. Pick the next step and build something small with it.
