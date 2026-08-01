# 🛠️ Troubleshooting Guide

Common errors encountered while setting up, wiring, or running the Automated Parking System, and how to fix them.

---

### ❌ ESP32 Board Not Found in Arduino IDE

**Cause:** ESP32 board package not installed, or the Boards Manager URL is missing/incorrect.

**Solution:**
- Add the ESP32 Boards Manager URL under **File → Preferences**:
  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
- Install the ESP32 package via **Tools → Board → Boards Manager**.

---

### ❌ `fatal error: ESP32Servo.h: No such file or directory`

**Cause:** The `ESP32Servo` library is not installed.

**Solution:**
- Install it via **Sketch → Include Library → Manage Libraries**, searching for `ESP32Servo`.
- Do not substitute the standard Arduino `Servo.h` — it isn't reliably compatible with the ESP32's PWM implementation.

---

### ❌ Upload Fails / "Failed to connect to ESP32"

**Cause:** The ESP32 isn't entering bootloader mode automatically, which some DevKit boards require manually.

**Solution:**
- Hold the **BOOT** button on the board while clicking Upload, and release it once "Connecting..." appears in the console.
- Try a lower upload speed under **Tools → Upload Speed** (e.g., 115200).
- Confirm the correct **Port** is selected under **Tools → Port**.

---

### ❌ Serial Monitor Shows Garbled / Unreadable Text

**Cause:** Baud rate mismatch between the Serial Monitor and the firmware.

**Solution:**
- Set the Serial Monitor baud rate to **115200**, matching `Serial.begin(115200)` in the code.

---

### ❌ Servo Motor Doesn't Move, Jitters, or Resets the ESP32

**Cause:** Insufficient current supply — servos can draw more current than the ESP32's onboard regulator can provide, especially both motors at once.

**Solution:**
- Power the servo motors from an external, stable 5V supply rather than the ESP32's onboard 3.3V/5V pin.
- Ensure a common ground between the ESP32 and the external power supply.
- Add a decoupling capacitor across the servo power rails if jitter persists.

---

### ❌ IR Sensor Always Reads HIGH or Always LOW

**Cause:** Detection threshold/distance not calibrated, or sensor orientation is wrong.

**Solution:**
- Adjust the onboard potentiometer on the IR sensor module to tune sensitivity/detection range.
- Confirm the sensor's `OUT` pin (not `VCC`/`GND`) is wired to the correct ESP32 GPIO.
- Test the sensor in isolation with a simple sketch that just prints `digitalRead()` values to confirm correct wiring before integrating it into the full system.

---

### ❌ Gate Opens/Closes Repeatedly for a Single Vehicle

**Cause:** This is exactly what the `sensorNTriggered` debounce flags are designed to prevent — if it's still happening, the flag logic may have been modified, or the sensor is producing noisy/flickering HIGH-LOW transitions.

**Solution:**
- Double-check that the `if (sensorValueX == HIGH && !sensorXTriggered)` / `else if (sensorValueX == LOW)` structure hasn't been altered.
- If the sensor itself is flickering near its detection threshold, add a short hardware or software debounce delay, or adjust sensor sensitivity.

---

### ❌ Occupied Slot Count Goes Negative or Exceeds `maxSlots`

**Cause:** A sensor is being triggered without a corresponding, correctly paired entry/exit event — for example, a false trigger on Sensor 3 without ever having incremented via Sensor 1.

**Solution:**
- Confirm the exit-side check `if (occupiedSlots > 0)` in the entry/exit logic is intact.
- In testing, always trigger sensors in the correct entry (1→2) or exit (3→4) sequence to simulate realistic vehicle movement.

---

### ❌ Python/Serial Tools Can't Read the ESP32 Output

**Cause:** Not applicable to this project directly — this firmware only requires the Arduino Serial Monitor. If you're building an external tool to read the serial output (e.g., for a future dashboard), ensure it uses the same baud rate (115200) and correct COM/serial port.

---

If your issue isn't listed here, check the Arduino IDE's console output for the full error message and consult the [ESP32 Arduino core documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/) or open a GitHub Issue with details and steps to reproduce.
