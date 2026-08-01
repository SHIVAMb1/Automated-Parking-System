# 📘 Project Report — Automated Parking System

**AISSMS Institute of Information Technology (IOIT)**
Department of Electronics & Telecommunication
An Autonomous Institute Affiliated to Savitribai Phule Pune University

**Project:** Automated Parking System
**Students:** Atharva Pednekar (PRN 23310004), Shivam Borane (PRN 23310015)

---

## Abstract

Rising urbanization and vehicle ownership have made efficient parking management increasingly important. This project presents an **Automated Parking System** that combines an ESP32 microcontroller, IR sensors, and servo motors to manage 10 (or more) parking slots without human intervention. The system detects vehicle entry and exit, updates parking availability in real time, and digitally displays the number of available slots on a serial monitor. Before granting entry, it verifies slot availability and controls servo-driven barriers accordingly, ensuring smooth, autonomous operation. The result is a practical, scalable solution that supports smart-city goals — improving parking convenience, reducing human error, and optimizing use of limited space.

---

## Introduction

As urbanization increases, managing parking spaces has become a significant challenge due to the growing number of vehicles on the road. Traditional, manually-managed parking methods often lead to inefficiency, long search times for drivers, and general frustration. This project addresses these issues with an Automated Parking System built around modern embedded technology.

The system combines an **ESP32 microcontroller**, **IR sensors**, and **servo motors** to automate vehicle entry and exit across a 10-slot parking area. The ESP32 processes sensor input to control gate opening/closing, ensuring smooth, unattended operation. IR sensors detect the presence of vehicles at key checkpoints, while servo motors physically operate the entry and exit gates based on that sensor data.

Parking slot availability is tracked continuously and shown on a serial monitor: the count decreases when a car enters and increases when a car exits. If no slots remain, the entry gate stays closed. This offers a scalable, cost-effective, and efficient approach to parking management that helps optimize space usage and reduce congestion in urban settings.

---

## Problem Statement

In urban areas, finding an available parking space is a growing challenge as vehicle numbers rise. Traditional parking systems are inefficient and often rely on manual management, contributing to traffic congestion and wasted time for drivers. There is a clear need for an automated solution capable of efficiently monitoring parking availability, optimizing space usage, and reducing human error. This project addresses that need by developing an Automated Parking System that manages parking slots, tracks availability in real time, and automates vehicle entry and exit.

---

## Literature Survey

Automated parking systems have drawn increasing research attention as a response to growing urban parking challenges. Several relevant themes emerge from prior work:

**Automated Parking Systems (APS) Overview** — A significant body of research focuses on systems that remove the need for human intervention, using sensors, microcontrollers, and motors to automate parking and retrieval. Some studies further explore IoT and AI integration for dynamic slot optimization, with real deployments seen in malls, airports, and office buildings.

**IR Sensors for Vehicle Detection** — Infrared sensors are a common, low-cost, and reliable choice for detecting vehicle presence by sensing the interruption of an infrared beam. K. R. Kumar and R. B. Chandran (2016) demonstrated a system using IR sensors to detect vehicles and control gate access based on slot availability.

**Microcontroller-Based Systems** — Multiple studies center on Arduino- or ESP32-based systems as the control unit for sensors and actuators, using servo motors to automate gates. D. Gupta and P. S. R. S. Sarma (2018) built an Arduino-based prototype for monitoring and controlling vehicle access, while S. Shah and S. B. Patel (2020) explored the ESP32 specifically for its versatility and built-in Wi-Fi/Bluetooth capability for remote monitoring.

**Wi-Fi-Enabled Parking** — Research on IoT-enabled parking management highlights the benefits of adding Wi-Fi/Bluetooth connectivity, allowing cloud-based, real-time slot availability information to reach users via mobile apps or web interfaces. M. G. Tare and A. J. P. Joshi (2017) demonstrated Wi-Fi-based remote availability checks that improve user experience and reduce search time.

**Scalability and Efficiency** — A. R. S. Subramanian et al. (2019) explored machine-learning-based real-time optimization of slot allocation based on demand patterns, improving space utilization as vehicle numbers grow.

**Cost-Effective Solutions** — S. P. Pandya and R. S. Rana (2020) designed a low-cost system using IR sensors, an ESP8266 microcontroller, and servo motors, emphasizing affordable technology for scalable parking solutions in budget-constrained cities.

---

## Objectives

1. Design and implement an ESP32-based control system to automate parking gate operations.
2. Use IR sensors to reliably detect vehicle entry and exit events.
3. Use servo motors to physically control entry and exit barriers.
4. Track parking slot occupancy in real time and prevent entry when the lot is full.
5. Display live slot availability via the Serial Monitor.
6. Produce a low-cost, scalable prototype suitable for further development toward smart-city parking solutions.

---

## Block Diagram of Working Steps

<p align="center">
  <img src="../Hardware/Block_Diagram.png" alt="Block Diagram of Working Steps" width="650">
</p>

The signal and control flow proceeds: **ESP32 (Microcontroller) → IR Sensor 1 (Motor Opening Sensor) → Servo Motor 1 → IR Sensor 2 (Motor Closing Sensor) → IR Sensor 3 (Motor Opening Sensor) → Servo Motor 2 → IR Sensor 4 (Motor Closing Sensor) → Serial Monitor (Shows Slots Data)**.

---

## Methodology

The Automated Parking System is a fully functional, real-time solution integrating an **ESP32 microcontroller**, **IR sensors**, and **servo motors** to deliver automated parking without requiring human intervention.

### System Components

**1. ESP32 Microcontroller**
- **Function:** Acts as the main control unit — processes sensor data and controls the servo motors that operate the gates.
- **Features:** Dual-core processor for multitasking; built-in Wi-Fi and Bluetooth connectivity; versatile GPIO pins for interfacing with sensors and motors.
- **Role in the System:** Handles real-time input/output operations and reports parking slot availability to the serial monitor.

**2. IR Sensors** — Four IR sensors are strategically placed:
- **IR Sensor 1:** Detects a vehicle approaching the entry gate.
- **IR Sensor 2:** Confirms the vehicle has passed through the entry gate.
- **IR Sensor 3:** Detects a vehicle approaching the exit gate.
- **IR Sensor 4:** Confirms the vehicle has exited the parking area.

Each sensor outputs a HIGH (1) or LOW (0) digital signal to the ESP32 depending on whether a vehicle is present.

**3. Servo Motors** — Two servo motors are used:
- **Servo Motor 1:** Controls the entry gate.
- **Servo Motor 2:** Controls the exit gate.

Each motor opens or closes its gate at a specific angle based on signals from the ESP32.

**4. Serial Monitor** — Displays real-time parking slot availability and sensor status, providing a simple way to observe the system's operation.

---

## Working of the System

**Vehicle Entry:**
1. IR Sensor 1 detects a vehicle approaching the entry gate.
2. The ESP32 checks whether parking slots are available.
3. If slots are available, Servo Motor 1 opens the entry gate.
4. Once the vehicle passes, IR Sensor 2 confirms entry and the gate closes.
5. The occupied slot count updates and is reported on the serial monitor.

**Vehicle Exit:**
1. IR Sensor 3 detects a vehicle approaching the exit gate.
2. Servo Motor 2 opens the exit gate.
3. Once the vehicle exits, IR Sensor 4 confirms departure and the gate closes.
4. The available slot count increases and is reported on the serial monitor.

**Real-Time Monitoring:**
- The ESP32 continuously reports parking slot availability to the serial monitor.
- If no slots remain available, the entry gate stays closed, preventing further vehicles from entering.

### Features of the System
- **Real-Time Parking Management:** Availability updates instantly as vehicles enter or exit.
- **Automated Gate Control:** Servo motors ensure precise, smooth gate operation tied to vehicle detection.
- **Error Reduction:** Removing human intervention minimizes management errors.
- **Scalability:** The system can be expanded to more slots by adding sensors and adjusting the firmware.

---

## Circuit Connections

<p align="center">
  <img src="../Hardware/Circuit_Design.png" alt="Circuit Wiring Diagram" width="700">
</p>

**ESP32 Microcontroller** — the central processing unit, managing sensor inputs and controlling the servo motors. Powered from a 5V supply, with the following GPIO roles:
- IR Sensor 1 → detects vehicle entry
- IR Sensor 2 → confirms vehicle entry / closes entry gate
- IR Sensor 3 → detects vehicle exit
- IR Sensor 4 → confirms vehicle exit / closes exit gate
- Servo Motor 1 (entry gate) → PWM signal from a dedicated GPIO
- Servo Motor 2 (exit gate) → PWM signal from a separate GPIO

**IR Sensors** — each connects to a dedicated ESP32 GPIO pin and is placed strategically: Sensor 1 at the entry point, Sensor 2 just after the entry gate, Sensor 3 at the exit point, and Sensor 4 just after the exit gate. Each returns digital HIGH when a vehicle is detected and LOW when clear.

**Servo Motors** — Servo Motor 1 controls the entry gate based on IR Sensors 1 & 2; Servo Motor 2 controls the exit gate based on IR Sensors 3 & 4.

**Serial Monitor Output** — the ESP32 connects to a computer via USB, streaming real-time parking availability data.

### Pin Configuration

| Component | GPIO Pin |
|---|:---:|
| IR Sensor 1 (Entry Detection) | GPIO 5 |
| IR Sensor 2 (Entry Confirmation) | GPIO 18 |
| IR Sensor 3 (Exit Detection) | GPIO 27 |
| IR Sensor 4 (Exit Confirmation) | GPIO 26 |
| Servo Motor 1 (Entry Gate) | GPIO 19 |
| Servo Motor 2 (Exit Gate) | GPIO 25 |

- **Serial Monitor:** Connected via USB, provides real-time output at 115200 baud.
- **Power Supply:** IR sensors and servo motors are powered from a shared 5V rail.

---

## Arduino Code

The full firmware source is provided in [`../Arduino code/AutomatedParkingSystem/AutomatedParkingSystem.ino`](../Arduino%20code/AutomatedParkingSystem/AutomatedParkingSystem.ino).

**Code overview:**
- Uses the `ESP32Servo` library to drive both gate servos.
- Defines four sensor GPIOs (`sensorPin1`–`4`) and two servo GPIOs (`servoPin1`, `servoPin2`).
- Tracks `occupiedSlots` against a `maxSlots` constant (10 by default).
- Uses per-sensor `triggered` boolean flags to debounce each sensor and avoid double-counting a single vehicle pass.
- In `setup()`, initializes serial communication at 115200 baud, attaches both servos, configures sensor pins as inputs, closes both gates (angle 0), and prints the initial slot count.
- In `loop()`, reads all four sensors each cycle and:
  - Opens the entry gate and increments `occupiedSlots` when Sensor 1 detects a new vehicle and slots remain (otherwise logs that the lot is full).
  - Closes the entry gate when Sensor 2 confirms the vehicle has passed.
  - Opens the exit gate and decrements `occupiedSlots` when Sensor 3 detects a departing vehicle (with a check to avoid going below zero).
  - Closes the exit gate when Sensor 4 confirms the vehicle has left.
  - Prints the updated slot count to the Serial Monitor after each entry/exit event.

For the complete algorithm breakdown, see [`Working_Principle.md`](Working_Principle.md).

---

## Conclusion

The Automated Parking System demonstrates the effective integration of modern hardware and software to build a practical, user-friendly parking management solution. By automating vehicle detection, barrier control, and availability tracking, the system removes human error, improves operational efficiency, and reduces the time drivers spend parking.

Using IR sensors, an ESP32 microcontroller, and servo motors keeps the design cost-effective while remaining robust, making it suitable for deployment in settings such as malls, offices, and residential complexes. Real-time monitoring further improves usability.

Beyond addressing today's parking challenges, this project contributes to the broader goal of building smarter cities through efficient resource management. Future development could add mobile app integration, payment systems, and AI-based slot optimization to make the system even more versatile and impactful — see [`Future_Improvements.md`](Future_Improvements.md) for the full roadmap.
