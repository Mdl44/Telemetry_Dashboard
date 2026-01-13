# Wireless Telemetry Dashboard for Sim-Racing

## 1. Description
The **Wireless Telemetry Dashboard** is a real-time hardware companion built on the ESP32 platform. It bridges the gap between simulation and reality by extracting live telemetry data from **F1 23** (or any compatible simulators) via Wi-Fi.

Throughout every session, from Free Practice to the Race, it serves as your race engineer:

- **Connectivity:** Captures raw telemetry data streams via UDP Broadcast, parsing binary packets in strict compliance with the official F1 23 data output specifications to ensure maximum accuracy.
- **Visualization:** Renders critical driving data such as Speed, Gear, and Lap Times on a TFT display, providing essential information at a glance while racing.
- **Feedback:** Features an RGB LED bar synchronized with engine RPM, acting as a physical shift-light indicator for optimal gear changes.

Inspired by the capabilities of **SimHub**, this project transitions telemetry processing from PC software to a dedicated embedded system.

---

## 2. Key Features

- **Wireless Data Acquisition**  
  Eliminates the need for USB data cables by utilizing the ESP32’s Wi-Fi capabilities to capture UDP Broadcast packets directly from the local network.

- **Real-Time Packet Decoding**  
  Parses binary data structures (Little Endian format) to interpret game physics and telemetry, translating raw bits into meaningful driving metrics.

- **Dynamic Shift Lights**  
  Maps engine RPM to a physical RGB LED bar, providing real-time visual cues for optimal gear changes synchronized with the virtual car’s performance.

- **Data Filtering**  
  Automatically identifies and isolates the player’s vehicle data from the full 22-car grid, ensuring that only relevant telemetry is displayed while ignoring AI or opponent traffic.

---

## 3. Hardware Architecture

- **Microcontroller:** ESP32 Development Board  
- **Display:** TFT LCD (SPI Interface)  
- **LEDs:** RGB LED Bar (WS2812B)  
- **Buzzer:** Used for shift-tone sounds  
- **Input:** Push Button  

---

## System Design Questions

### Q1 – What is the system boundary?
The system is the physical dashboard.

- **Inside the system:**  
  ESP32 microcontroller, TFT display, RGB LED bar, buzzer, and the firmware handling the Wi-Fi/UDP stack.
- **Outside the system:**  
  The PC running F1 23 (data source) and the user (driver).

---

### Q2 – Where does intelligence live?
The intelligence lives on the **ESP32 microcontroller**. While the PC calculates the game physics, the ESP32 is responsible for protocol translation. It captures raw binary packets, filters the player-specific data from a 22-car array, decodes the Little Endian format, and displays the results on the LCD.

---

### Q3 – What is the hardest technical problem?
**Concurrency and latency minimization.**  
The system must handle a high-frequency stream of UDP packets (up to 60 Hz) while simultaneously updating an SPI display and driving real-time LED animations.

---

### Q4 – What is the minimum demo?
The minimum demo is **telemetry reception**.  
The device must successfully connect to the local Wi-Fi network, receive data from the telemetry source (game or Python script), and update the RPM LEDs and speed value on the display in real time.

---

### Q5 – Why is this not just a tutorial?
This is not a tutorial-based project because the core problems do not have ready-made solutions. The system required understanding an official telemetry specification, decoding raw binary data, and designing a real-time embedded architecture.
