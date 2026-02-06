# Wireless Telemetry Dashboard for Sim-Racing

## 1. Description
The **Wireless Telemetry Dashboard** is a high-performance hardware companion built on the ESP32 platform. It serves as a dedicated race engineer, bridging the gap between simulation and reality by extracting live telemetry data from **F1 23** via Wi-Fi.

Unlike generic dashboards, this system functions as a standalone Access Point, eliminating connection hassles. It processes complex telemetry packets locally to provide features typically found only in PC software, such as **Live Delta timing** and **Tyre Thermal auditing**, directly on your steering wheel or desk.

---

## 2. Key Features

- **Zero-Latency Visualization (MVC Architecture)**  
  Built on a Model-View-Controller pattern with "dirty-tracking" rendering. Only pixels that change are redrawn, ensuring high refresh rates (30-60 FPS) on the SPI display, even with heavy data processing.

- **Dual-Buffer Live Delta System**  
  Implements a custom interpolation algorithm that records your best lap into a reference buffer (300 points) and compares your current position in real-time. This provides an F1-style "Live Delta" accurate to the millisecond, updating continuously through the lap.

- **Multi-Page Interface (4 Screens)**  
  Cycle through four specialized screens using a physical button, covering everything from hot-lapping timing to endurance race strategy.

- **Standalone Operation**  
  The ESP32 creates its own Wi-Fi network (`Telemetry_Dashboard`), so no router configuration is needed. Just connect the PC to the dashboard's network.

- **Active Feedback**  
  - **RPM Bar:** 8-LED NeoPixel strip synchronized with the car's rev counters and shift points.
  - **Buzzer:** Audio cues for gear shifts and DRS availability.

---

## 3. Dashboard Interface

The system is organized into four distinct screens, capable of displaying data for any of the 22 cars on track (defaulting to the player).

### Screen 1: Main Telemetry (Hotlap Focus)
Designed for the driver's primary line of sight.
- **Center:** Current Gear (Large), Speed, Engine RPM.
- **Timing:** Live Delta (Interpolated), Last Lap, Difference to Car Ahead/Leader.
- **Live Bars:** Throttle and Brake application traces.
- **Status:** DRS availability, ERS deployment mode, Flag/Warning indicators.

### Screen 2: Tyre Thermal Monitor
complete analysis of the tyre contact patch divided into 4 quadrants.
- **Temperatures:** Surface, Inner carcass, and Brake temperatures per wheel.
- **Status:** Tyre Pressures (PSI), graphical wear bars, and structural damage percentages.

### Screen 3: Vehicle Status & Aero
Detailed damage report for pit-decisions.
- **Power Unit:** ICE, MGU-K, MGU-H wear levels and Engine Temperature.
- **Aerodynamics:** specific damage values for Front Left/Right Wings, Rear Wing, Floor, and Sidepods.
- **Components:** Gearbox condition and ERS store integrity.

### Screen 4: Strategic Overview
Race engineer summary for endurance management.
- **Session:** Weather forecast, Air/Track temps, Safety Car status.
- **Fuel:** Remaining laps calculation and fuel load (kg).
- **Stint Info:** Current Set Tyre Age (Laps) and Critical Engine status.
- **Lap History:** Best Lap vs Last Lap sector comparison.

---

## 4. Hardware Architecture

- **Microcontroller:** ESP32-WROOM-32
- **Display:** 2.4" TFT LCD (ILI9341 Driver, SPI: `CS=15`, `DC=2`, `RST=4`)
- **Visuals:** 8x WS2812B RGB LED Strip (`Pin 13`)
- **Audio:** Active Buzzer (`Pin 12`)
- **Control:** Momentary Push Button (`Pin 14`)
- **Power:** 5V via USB

---

## 5. Software Architecture

- **Protocol:** UDP Broadcast (Port `20777`) matching F1 23 "2023" Packet Format.
- **Connection:** ESP32 Access Point Mode (`SSID: Telemetry_Dashboard`, `PASS: 44168104`).
- **Data Handling:**
  - **Model:** Parses raw binary packets (Little Endian) into structured vehicle data.
  - **View:** Handles layout drawing and efficient partial updates.
  - **Controller:** Manages network stack, button debouncing, and buzzer logic.

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