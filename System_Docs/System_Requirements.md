# Project "Kraft-6M": 50MHz Digital RC Transceiver System
## Overall System Requirements & Project Specification

### 1. Executive Summary
The **Kraft-6M** project is a high-performance "Restomod" of a vintage Kraft 7-channel Series 70 transmitter. The goal is to replace the obsolete 72MHz analog electronics with a modern, high-power (1W) digital GFSK link operating in the **6-meter (50MHz) amateur radio band**. This system provides extreme range, robust obstacle penetration, and full 2-way telemetry while preserving the iconic aesthetic of the original gold Kraft hardware.

---

### 2. General System Specifications
- **Operating Frequency**: 50.0 - 54.0 MHz (Amateur 6m Band).
- **Control Protocol**: Digital GFSK (ExpressLRS-inspired logic).
- **Modulation**: Gaussian Frequency Shift Keying (GFSK).
- **Channels**: 7 proportional channels (16-bit resolution).
- **Telemetry**: 2-way link (RSSI, Battery Voltage, Link Quality).
- **Link Range**: >10 km (Line of Sight, theoretical).

---

### 3. Transmitter (TX) Requirements
The transmitter is housed in a modified Kraft 7 case.

- **Microcontroller**: ESP32-S3 (Dual-core, 16MB Flash).
- **Precision Input**: Dual **ADS1115** 16-bit ADCs for ultra-smooth stick and auxiliary control.
- **RF Modulator**: **Si5351A** Clock Generator (I2C controlled).
- **Power Amplifier (PA)**: **RD01MUS2** MOSFET providing **1.0 Watt (+30 dBm)** output.
- **Filtering**: Mandatory **7-pole Low Pass Filter (LPF)** to suppress harmonics below -50dBc.
- **User Interface**: 1.3" SH1106 OLED display for configuration and telemetry.
- **Power System**: 
    - 2S (7.4V) 2500mAh LiPo battery.
    - Integrated **USB-C Power Delivery (PD)** charging (12V input).
    - Precision buck-regulation (5V) and LDO (3.3V) logic rails.
- **Analog Interface**: JST-XH 2.54mm modular connectors for all stick potentiometers.

---

### 4. Receiver (RX) Requirements
A compact, high-sensitivity SDR-based receiver for aircraft installation.

- **Microcontroller**: ESP32-S3.
- **Front-End (LNA)**: **SPF5043Z** Low Noise Amplifier (<0.8dB Noise Figure).
- **Active Mixer**: **LT5560** active mixer for superior dynamic range and image rejection.
- **Intermediate Frequency (IF)**: 10.7 MHz ceramic filter for narrow-band interference rejection.
- **Servo Interface**: 8-channel standard 50Hz PWM bus.
- **Power Supply**: 
    - Draws from 5V/6V Servo bus (BEC).
    - **LC Pi-Filter** and **Reverse Polarity Protection** to isolate RF stages from servo noise.
- **Form Factor**: Compact 30x50mm 4-layer PCB.

---

### 5. Firmware & Software Requirements
- **Encoder**: 50Hz frame rate with CRSF-style packet structure.
- **Hopping**: Frequency hopping spread spectrum (FHSS) capability within the 6m band.
- **Trim System**: Full digital trim with non-volatile (EEPROM) storage.
- **Fail-Safe**: Programmable "Hold" or "Go-to-Preset" positions upon signal loss.
- **Telemetry Handler**: Real-time CRC8-checked packet decoding for on-screen battery and signal reporting.

---

### 6. Mechanical & Aesthetic Goals
- **Preservation**: No new external holes in the front face of the radio (except the OLED window).
- **Aesthetic**: Use the original mechanical switches and gimbal assemblies.
- **Ergonomics**: Maintain the weight and balance of the original Kraft 7.
- **Durability**: All internal electronics mounted on a rigid "Digital Deck" sub-chassis.

---

### 7. Regulatory Compliance (Ham Radio)
- **License**: Operation requires a valid **Amateur Radio Technician Class** license or higher.
- **Station ID**: Firmware must support periodic station identification (CW or digital) per FCC Part 97.

---
*Document Version: 1.0*
*Project Lead: ne9n*
