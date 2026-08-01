# 🧭 Installation Guide

This guide walks you through setting up and running the Automated Parking System firmware, from installing the Arduino IDE to uploading the sketch to your ESP32.

---

## 1. Install the Arduino IDE

1. Download the Arduino IDE from [arduino.cc/en/software](https://www.arduino.cc/en/software).
2. Install it for your operating system (Windows/macOS/Linux).

---

## 2. Add ESP32 Board Support

1. Open Arduino IDE → **File → Preferences**.
2. In "Additional Boards Manager URLs," add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Go to **Tools → Board → Boards Manager**, search for **esp32**, and install the package by Espressif Systems.
4. Select your specific board under **Tools → Board → ESP32 Arduino** (e.g., "DOIT ESP32 DEVKIT V1").

---

## 3. Install the Required Library

1. Go to **Sketch → Include Library → Manage Libraries**.
2. Search for **ESP32Servo** and install it (this library provides ESP32-compatible servo control, unlike the standard `Servo.h`).

---

## 4. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/Automated-Parking-System.git
cd Automated-Parking-System
```

Or download the repository as a ZIP from GitHub and extract it.

---

## 5. Open the Sketch

Open `firmware/AutomatedParkingSystem/AutomatedParkingSystem.ino` in the Arduino IDE. The `.ino` file must remain inside a folder with the exact same name — this is a hard requirement of the Arduino IDE, already satisfied by this repository's structure.

---

## 6. Wire the Hardware

Follow the wiring shown in [`../Hardware/Circuit_Design.png`](../Hardware/Circuit_Design.png) and the pin table in [`Components_List.md`](Components_List.md):

| Component | GPIO Pin |
|---|:---:|
| IR Sensor 1 (Entry Detection) | GPIO 5 |
| IR Sensor 2 (Entry Confirmation) | GPIO 18 |
| IR Sensor 3 (Exit Detection) | GPIO 27 |
| IR Sensor 4 (Exit Confirmation) | GPIO 26 |
| Servo Motor 1 (Entry Gate) | GPIO 19 |
| Servo Motor 2 (Exit Gate) | GPIO 25 |

Power both IR sensors and servos from a stable 5V supply, sharing a common ground with the ESP32.

---

## 7. Select Port and Upload

1. Connect the ESP32 to your computer via USB.
2. In Arduino IDE, select the correct **Port** under **Tools → Port**.
3. Click **Upload** (the arrow icon).
4. Wait for "Done uploading" in the console.

---

## 8. Open the Serial Monitor

1. Go to **Tools → Serial Monitor** (or press `Ctrl+Shift+M`).
2. Set the baud rate to **115200** (must match `Serial.begin(115200)` in the firmware).
3. You should see: `10 slots are available.`

Trigger IR Sensor 1 (simulate a vehicle) to see the slot count and gate behavior respond in real time.

---

## 9. Common Installation Issues & Solutions

| Issue | Solution |
|---|---|
| ESP32 board not listed under Tools → Board | Confirm the ESP32 Boards Manager URL was added correctly and the package installed successfully |
| `fatal error: ESP32Servo.h: No such file or directory` | Install the ESP32Servo library via Library Manager — the standard `Servo.h` does not work reliably on ESP32 |
| Upload fails / times out | Hold the **BOOT** button on the ESP32 while uploading, especially on some DevKit boards |
| Serial Monitor shows garbled text | Confirm the baud rate is set to **115200** to match the firmware |
| Servo doesn't move / jitters | Check power supply — servos can draw more current than the ESP32's onboard 3.3V/5V regulator can supply; use an external 5V source |
| Sensor always reads HIGH or LOW | Adjust the potentiometer on the IR sensor module to calibrate its detection threshold/distance |

For more detailed troubleshooting, see [`Troubleshooting.md`](Troubleshooting.md).

---

✅ You're all set! Head back to the [README](../README.md) for full usage instructions.
