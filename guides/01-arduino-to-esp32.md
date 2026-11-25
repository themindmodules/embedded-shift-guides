# Moving from Arduino to ESP32

You know Arduino. You can blink an LED, read a sensor, print to the serial monitor. The ESP32 is the
next step because it adds Wi-Fi, Bluetooth, two cores and a lot more speed, and you can keep using
the Arduino IDE while you learn it.

This guide is a path, not a manual. Do the steps in order.

## Step 1: Know what changes

| | Arduino Uno | ESP32 dev board |
|---|---|---|
| Logic level | 5 V | **3.3 V** (pins are not 5 V tolerant) |
| Clock | 16 MHz | up to 240 MHz, two cores |
| RAM | 2 KB | about 520 KB |
| Radio | none | Wi-Fi (2.4 GHz) and Bluetooth |
| ADC | 10 bit, clean | 12 bit, noisy and not linear |
| PWM | analogWrite on a few pins | LEDC peripheral, almost any pin |

The one that burns boards: **3.3 V logic**. A 5 V sensor output going straight into an ESP32 pin
can damage it. Use a level shifter or a voltage divider.

## Step 2: Set up the Arduino IDE

1. Open File, Preferences. In "Additional boards manager URLs" paste
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
2. Open Tools, Board, Boards Manager. Search "esp32" and install **esp32 by Espressif Systems**.
3. Plug in the board. Choose your board under Tools, Board, esp32 (for most cheap dev boards,
   "ESP32 Dev Module" works).
4. Pick the right COM port.

If the port doesn't show up, the board needs a USB driver (CP210x or CH340, depending on the board)
or the cable is charge only. Try another cable first. It's the most common problem.

## Step 3: First upload

```cpp
const int LED = 2;            // many boards have the LED on GPIO 2, check yours

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}
```

When you click upload and the log says `Connecting........`, some boards need you to **hold the BOOT
button** until it starts writing. Release it after that.

## Step 4: Learn the pin rules (before you wire anything)

- **GPIO 34, 35, 36, 39** are input only. No output, no internal pull-up.
- **GPIO 6 to 11** are connected to the flash chip. Never use them.
- **Strapping pins** (GPIO 0, 2, 5, 12, 15) decide how the chip boots. If you pull them the wrong way
  with a sensor or a button, the board may refuse to start. GPIO 12 is the classic trap.
- **ADC2 pins** don't work while Wi-Fi is on. Use ADC1 pins (GPIO 32 to 39) for analog readings.
- Keep each output pin to a small load (about 10 mA is a safe habit). Drive relays and motors with a
  transistor or a driver, not directly.

## Step 5: Redo the things you already know

1. **Serial:** `Serial.begin(115200);` the ESP32 prints its boot log at 115200, so use that baud.
2. **Analog read:** `analogRead()` gives 0 to 4095. For a better number use
   `analogReadMilliVolts(pin)`, which applies the chip's calibration.
3. **PWM:** newer core versions (3.x) have `analogWrite()`, or you can use `ledcAttach(pin, freq, bits)`
   then `ledcWrite(pin, duty)`. Older core versions (2.x) used `ledcSetup` and `ledcAttachPin`.
   If an example fails to compile, check which core version you have. This is the number one reason
   internet code doesn't work.
4. **I2C and SPI:** same libraries, but the default pins are different. Default I2C is SDA = GPIO 21,
   SCL = GPIO 22. You can also pass your own pins to `Wire.begin(sda, scl)`.

## Step 6: Use the new stuff

Do these three, one at a time:

1. **Wi-Fi:** connect to your router and print the IP.

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.begin("your-ssid", "your-password");
  while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print("."); }
  Serial.println(WiFi.localIP());
}

void loop() {}
```

2. **A tiny web server or MQTT message** to see data leave the board.
3. **Deep sleep:** wake up on a timer, read a sensor, send it, go back to sleep. The chip drops to
   microamps, but a dev board often stays at several milliamps because of the USB chip and the
   regulator. For real battery products you need a bare module and your own board.

## Step 7: Meet the second core

`loop()` runs on core 1. Wi-Fi runs on core 0. You can start your own tasks with FreeRTOS:

```cpp
void blinkTask(void *p) {
  pinMode(2, OUTPUT);
  for (;;) { digitalWrite(2, !digitalRead(2)); vTaskDelay(500 / portTICK_PERIOD_MS); }
}

void setup() {
  xTaskCreatePinnedToCore(blinkTask, "blink", 2048, NULL, 1, NULL, 0);
}
void loop() {}
```

This is where the ESP32 stops being a faster Arduino. Two things blink at their own pace without
`millis()` juggling.

## Step 8: Where to go next

- Read how the code actually gets onto the chip (the boot and flash process).
- Try **ESP-IDF** (Espressif's own framework) once the Arduino layer feels easy.
- If you need exact timing, low power or industrial reliability, move on to STM32.
  See `02-esp32-to-stm32.md`.

## Common problems

| Problem | Usual cause |
|---|---|
| `Connecting........` forever | Hold BOOT while uploading, check the cable and the COM port |
| Board resets again and again | Weak USB power, or a strapping pin held low or high |
| Code from the internet will not compile | Core 2.x versus 3.x API difference |
| Sensor reads random values | 5 V sensor on a 3.3 V pin, or ADC2 used with Wi-Fi |
| Wi-Fi does not connect | The ESP32 only does 2.4 GHz, not 5 GHz |
