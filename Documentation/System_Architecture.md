# 🏗️ System Architecture

This document describes the architecture of the Automated Parking System — its hardware components, their roles, and the data/control flow between them.

---

## Architectural Components

| Component | Responsibility |
|---|---|
| **ESP32 Microcontroller** | Central control unit — reads all sensors, drives both servos, and reports status over serial |
| **IR Sensor 1** | Detects a vehicle approaching the entry gate |
| **IR Sensor 2** | Confirms the vehicle has cleared the entry gate |
| **IR Sensor 3** | Detects a vehicle approaching the exit gate |
| **IR Sensor 4** | Confirms the vehicle has cleared the exit gate |
| **Servo Motor 1** | Physically opens/closes the entry barrier |
| **Servo Motor 2** | Physically opens/closes the exit barrier |
| **Serial Monitor** | Displays live slot availability and system events to the operator/developer |

---

## ASCII Block Diagram

```
                         ┌─────────────────────┐
                         │   ESP32 (Microcontroller) │
                         └───────────┬───────────┘
                                     │
        ┌────────────────────────────┼────────────────────────────┐
        ▼                            ▼                             ▼
┌───────────────┐          ┌───────────────────┐          ┌───────────────┐
│  IR Sensor 1     │          │   Servo Motor 1      │          │  IR Sensor 2     │
│ (Entry Detect)     │ ───────▶ │  (Entry Gate)          │ ───────▶ │ (Entry Confirm)   │
└───────────────┘          └───────────────────┘          └───────────────┘
                                     │
                                     ▼
                         ┌─────────────────────┐
                         │   occupiedSlots counter   │
                         └───────────┬───────────┘
                                     │
        ┌────────────────────────────┼────────────────────────────┐
        ▼                            ▼                             ▼
┌───────────────┐          ┌───────────────────┐          ┌───────────────┐
│  IR Sensor 3     │          │   Servo Motor 2      │          │  IR Sensor 4     │
│ (Exit Detect)      │ ───────▶ │  (Exit Gate)           │ ───────▶ │ (Exit Confirm)    │
└───────────────┘          └───────────────────┘          └───────────────┘
                                     │
                                     ▼
                         ┌─────────────────────┐
                         │      Serial Monitor       │
                         │   (Shows Slots Data)        │
                         └─────────────────────┘
```

---

## Data Flow Summary

```
Vehicle Approaches Entry
   → IR Sensor 1 (digital HIGH)
   → ESP32 checks occupiedSlots < maxSlots
   → Servo Motor 1 opens (90°) + occupiedSlots++
   → IR Sensor 2 confirms passage → Servo Motor 1 closes (0°)
   → Serial Monitor updated

Vehicle Approaches Exit
   → IR Sensor 3 (digital HIGH)
   → ESP32 checks occupiedSlots > 0
   → Servo Motor 2 opens (90°) + occupiedSlots--
   → IR Sensor 4 confirms passage → Servo Motor 2 closes (0°)
   → Serial Monitor updated
```

---

## Design Notes

- **State machine per gate:** Each gate (entry/exit) is effectively a small state machine driven by two sensors — one to trigger opening, one to trigger closing — keeping the logic simple and predictable.
- **Debounce flags:** Each sensor has a corresponding `triggered` boolean to ensure a single physical vehicle pass only fires one open/close event, rather than repeatedly re-triggering while the vehicle remains in the sensor's field of view.
- **Centralized slot counter:** A single `occupiedSlots` variable (bounded by `maxSlots`) is the sole source of truth for parking availability, keeping the logic straightforward to reason about and extend.
- **Extensibility:** Because each entry/exit lane follows the same two-sensor + one-servo pattern, adding more lanes (for a larger or multi-entrance facility) mainly means duplicating this pattern with new GPIO assignments.

For future integration with a mobile app or cloud dashboard, the **Serial Monitor** block above would be replaced or supplemented by a **Wi-Fi/Cloud Communication Module**, leveraging the ESP32's built-in wireless capability — see [`Future_Improvements.md`](Future_Improvements.md).
