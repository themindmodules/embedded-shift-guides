# Tips and tricks

Small things that save hours. Keep this open while you work.

## Before you write code

1. **Read the pin table first.** Half of all "my code is broken" problems are a pin that's
   something else (boot pin, flash pin, a different alternate function).
2. **Check the supply.** A weak USB port or a thin cable causes resets that look like software bugs.
   Measure the 3.3 V rail.
3. **One change at a time.** Change one thing, test it. If you changed five things and it broke, you
   learned nothing.
4. **Put a pull-up on every open drain line** (I2C needs two resistors, about 4.7k each).

## Debugging

5. **Toggle a spare pin** at the start and end of a piece of code, and look at it with a logic
   analyser. It's the quickest way to measure how long something takes.
6. **Print less, look more.** A debugger or a logic analyser shows more than a hundred print lines.
7. **If nothing happens, check the clock enable.** On STM32 a peripheral without its clock ignores
   every write, with no error.
8. **Compare with a known good example** from the vendor. Change one line at a time until yours
   breaks.
9. **Write down the question before you ask the internet.** "What did I expect, what did I get, what
   did I change last?" Half the time you find the answer while writing it.

## C habits for embedded

10. **`volatile`** for anything that an interrupt or the hardware can change.
11. **Keep interrupt handlers short.** Set a flag, do the work in the main loop.
12. **Don't use `delay()` for timing that matters.** Use a timer or a millisecond counter and
    compare differences:
    ```c
    if ((uint32_t)(now - last) >= 1000u) { last = now; /* once per second */ }
    ```
    The subtraction still works when the counter wraps around.
13. **Avoid `float` and `printf("%f")` on small chips.** Use integers in milli-units, for example
    23375 for 23.375 degrees.
14. **Never use the same variable for two meanings.** Name it for what it holds, with the unit:
    `timeout_ms`, `vbat_mv`.

## Register level

15. **Clear a field before you set it.** `REG &= ~(mask); REG |= value;`
16. **Use BSRR instead of ODR** to set or clear one pin. It can't disturb the other pins and it's
    safe if an interrupt happens at the same time.
17. **Read the register back** after enabling a clock. A dummy read adds the short delay the
    hardware needs.
18. **Look at what the library did.** Open the HAL function, find the register writes, and copy the
    ones that matter.

## ESP32 specific

19. **Hold BOOT** while uploading if you see `Connecting........`.
20. **Use ADC1, not ADC2,** when Wi-Fi is on.
21. **Core versions matter.** If an example fails, check whether it was written for core 2.x or 3.x.
22. **Add a 100 uF capacitor** close to the board's 3.3 V if the board resets when Wi-Fi starts.
    The radio pulls short bursts of current.

## STM32 specific

23. **Learn the clock tree.** Wrong clocks are behind wrong baud rates, wrong PWM frequency and
    wrong timings.
24. **Use the debugger.** Breakpoints, Live Expressions and the SFR view are the main reason to
    move to STM32.
25. **Read the errata sheet** for your exact chip when something behaves oddly. It lists known
    silicon problems and the workarounds.

## Hardware habits

26. **Decoupling:** one 100 nF capacitor right next to every supply pin of every chip.
27. **Keep a spare board.** You'll damage one. It's cheaper to own two.
28. **Label your jumper wires and photograph a working setup.** Future you needs it.
29. **Version control your firmware** from day one, even for a tiny project.
30. **Write a note when something finally works:** what the problem was, and what fixed it.
