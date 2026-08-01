# ⚙️ Working Principle

This document explains, step by step, how the Automated Parking System firmware behaves from power-on through repeated vehicle entry/exit cycles.

---

## Step 1: System Initialization (`setup()`)

On power-up, the ESP32:
1. Starts serial communication at **115200 baud**.
2. Attaches `servo1` (entry gate) and `servo2` (exit gate) to their respective GPIO pins.
3. Configures all four IR sensor pins as digital inputs.
4. Commands both servos to angle **0°** (closed position).
5. Prints the initial state: `"10 slots are available."`

---

## Step 2: Continuous Sensor Polling (`loop()`)

On every loop iteration, the ESP32 reads the digital state of all four IR sensors (`sensorValue1`–`4`) and evaluates four independent event blocks — entry detection, entry-gate close, exit detection, and exit-gate close — each guarded by a "triggered" flag to avoid re-processing the same vehicle multiple times as it lingers in front of a sensor.

---

## Step 3: Vehicle Entry Detection

When **IR Sensor 1** goes HIGH for the first time (`sensor1Triggered` was previously `false`):
1. The flag is set to `true` to prevent repeat triggers.
2. If `occupiedSlots < maxSlots`:
   - **Servo Motor 1** rotates to 90° (gate opens).
   - `occupiedSlots` increments by 1.
   - The system prints the updated number of available slots.
3. Otherwise (lot full): the system prints `"Vehicle detected - All slots are full"` and the gate stays closed.
4. A 2-second delay allows the vehicle to begin moving through before the next check.

When Sensor 1 goes back LOW (vehicle has moved past the beam), `sensor1Triggered` resets to `false`, ready for the next vehicle.

---

## Step 4: Entry Gate Auto-Close

When **IR Sensor 2** (positioned just past the entry gate) goes HIGH for the first time:
1. `sensor2Triggered` is set to `true`.
2. **Servo Motor 1** rotates back to 0° (gate closes).
3. A 2-second delay follows before the flag resets on the next LOW reading.

This ensures the gate only closes once the vehicle has fully cleared the entry point.

---

## Step 5: Vehicle Exit Detection

When **IR Sensor 3** goes HIGH for the first time:
1. `sensor3Triggered` is set to `true`.
2. If `occupiedSlots > 0`:
   - **Servo Motor 2** rotates to 90° (exit gate opens).
   - `occupiedSlots` decrements by 1.
   - The system prints the updated available-slot count.
3. Otherwise (no vehicles logged as parked): the system prints `"Vehicle detected - All slots are empty"`.
4. A 2-second delay follows.

---

## Step 6: Exit Gate Auto-Close

When **IR Sensor 4** goes HIGH for the first time:
1. `sensor4Triggered` is set to `true`.
2. **Servo Motor 2** rotates back to 0° (gate closes).
3. A 2-second delay follows before the flag resets.

---

## Step 7: Loop Delay

At the end of each `loop()` pass, a short **100ms delay** paces the sensor-polling rate, keeping the system responsive without excessive CPU/serial overhead.

---

### 🔁 Summary Flow

```
Power On → Initialize Servos & Sensors → Close Both Gates → Print Initial Slot Count
   → [Loop] Read All 4 Sensors
       → Sensor 1 HIGH (new) → Check Slots → Open Entry Gate + Increment Count (or "Full")
       → Sensor 2 HIGH (new) → Close Entry Gate
       → Sensor 3 HIGH (new) → Check Slots → Open Exit Gate + Decrement Count (or "Empty")
       → Sensor 4 HIGH (new) → Close Exit Gate
   → Repeat
```
