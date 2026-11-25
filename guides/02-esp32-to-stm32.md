# Moving from ESP32 to STM32

ESP32 is great for anything that needs Wi-Fi or Bluetooth. STM32 is what you reach for when the
job needs exact timing, low power, many peripherals, or a product that has to run for years. Most
industrial boards use a chip from this class.

You don't leave the ESP32 behind. Many products use both: an STM32 for the real-time work and an
ESP32 for the wireless part.

## Step 1: Know when to switch

Move to STM32 when you need:

- Timing you can predict to the microsecond (motor control, power electronics, measurement)
- Low power that you can actually measure and tune
- Many timers, ADCs, DMA channels or a fast ADC
- A chip family you can buy for ten years and a proper debugger

Stay on ESP32 when the job is mostly Wi-Fi, a web page, or a quick prototype.

## Step 2: Pick one board

For learning, one of these (all have a built-in programmer):

| Board | Why |
|---|---|
| NUCLEO-F411RE | Cortex-M4, 100 MHz, plenty of examples, the same board used in my register level projects |
| NUCLEO-G474RE | More analog and timers, good for power electronics |
| NUCLEO-L432KC | Small and low power, Arduino Nano shape |

Don't start with a bare chip. Buy a Nucleo, because its ST-LINK programmer and debugger are built in.

## Step 3: Install the tools

1. **STM32CubeIDE** (editor, compiler and debugger in one). It already contains CubeMX.
2. **STM32CubeProgrammer** if you want to flash a file without the IDE.
3. Install the ST-LINK driver if Windows asks for it.

Download them from `st.com`. You need a free account.

## Step 4: First project in CubeIDE

1. File, New, STM32 Project. Open the **Board Selector** tab and pick your Nucleo.
2. Say yes to "initialise all peripherals with their default mode".
3. The LED already shows up as `LD2`. In `main.c`, inside the `while (1)` loop, add:

```c
HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
HAL_Delay(500);
```

4. Click the green debug button. The board is flashed over SWD, no BOOT button, no waiting.
5. Press resume. The LED blinks.

## Step 5: Learn the new ideas, in this order

| Order | Idea | What to build |
|---|---|---|
| 1 | The **clock tree**. Everything depends on it | Change the system clock in CubeMX and see what else changes |
| 2 | GPIO and interrupts | Button toggles an LED in an interrupt |
| 3 | UART | Print to the PC with `HAL_UART_Transmit` |
| 4 | Timers | PWM output and a precise periodic interrupt |
| 5 | ADC with DMA | Read a sensor without the CPU touching each sample |
| 6 | I2C and SPI | Talk to a real sensor chip |
| 7 | Low power modes | Sleep, stop, and wake on an event |

Spend more time on steps 1 to 4 than you think you need.

## Step 6: What is different from ESP32

| ESP32 / Arduino | STM32 |
|---|---|
| `setup()` and `loop()` are given to you | You write `main()`, and CubeMX generates the start-up code |
| Libraries hide the hardware | HAL, LL or direct registers, your choice |
| Upload over serial | Flash and debug over **SWD** with a real debugger |
| `Serial.print` | UART, or SWV/ITM trace through the debugger |
| `delay()` | `HAL_Delay()` uses SysTick. Use timers for real timing |
| FreeRTOS is already there | Add FreeRTOS from CubeMX if you need it |
| Wi-Fi built in | None. Add an ESP32 module on a UART, or use a chip with Ethernet |

## Step 7: Use the debugger from day one

This is the best part of moving over.

1. Set a breakpoint, step line by line.
2. Open **Live Expressions** and watch a variable change while the code runs.
3. Open the **SFRs** view and see the actual peripheral registers.

You'll stop using `printf` for most debugging.

## Step 8: Three layers, and when to use each

| Layer | What it is | Use it for |
|---|---|---|
| HAL | ST's easy library. Portable, a bit heavy | Getting something working fast |
| LL | Thin wrappers over the registers | When HAL is too slow or too big |
| Registers | Direct reads and writes | Understanding the chip, tight code |

Start with HAL, then read what HAL does inside. Then try the register level yourself.
See `03-start-register-level.md`.

## Common problems

| Problem | Usual cause |
|---|---|
| Debugger cannot connect | Wrong board selected, or cable is charge only. Update the ST-LINK firmware |
| Code does nothing after flashing | A peripheral clock was never enabled. Check CubeMX |
| UART prints garbage | The clock tree changed and the baud rate setting did not follow |
| A pin does nothing | The pin is set to another alternate function. Look at the pin view in CubeMX |
| Hard fault | Stack too small, null pointer, or a bad interrupt handler name |
