# Kraft-6M: Firmware Implementation & Protocol Guide

This document details the software logic required to run the 50MHz digital RC link using the ESP32-S3 and Si5351A.

## 1. Modulation Logic (Si5351A GFSK)
Unlike 2.4GHz chips (LoRa/CC2500), the Si5351A is a clock generator. To perform **Gaussian Frequency Shift Keying (GFSK)**, we must "pull" the frequency of the oscillator in real-time.

### 1.1 Bit Representation
- **Center Frequency ($f_c$):** 50.100 MHz
- **Frequency Deviation ($\Delta f$):** ±5 kHz
- **Logic '1':** $f_c + 5$ kHz
- **Logic '0':** $f_c - 5$ kHz

### 1.2 Implementation
We use the Si5351's **VCXO (Voltage Controlled Crystal Oscillator)** capability or rapidly update the `MultiSynth` registers via I2C.
- **Data Rate:** 2400 bps or 4800 bps (to keep bandwidth narrow and range high).
- **Encoding:** Manchester encoding or 8b/10b to ensure DC balance.

---

## 2. Packet Structure (24 Bytes)
Each frame transmitted every 20ms (50Hz) follows this structure:

| Byte | Field | Description |
| :--- | :--- | :--- |
| 0-1 | **Sync Word** | 0x1D4A (Used by RX to find start of frame) |
| 2-17| **Channels** | 8 Channels x 16-bits (from ADS1115) |
| 18 | **Flags** | Failsafe bit, Telemetry request bit |
| 19-22| **ID** | 32-bit Unique Transmitter ID |
| 23 | **CRC8** | Cyclic Redundancy Check (Polynomial: 0x07) |

---

## 3. Transmitter (TX) Logic Flow
1.  **Read ADCs**: Poll both ADS1115 modules for stick and aux knob positions.
2.  **Apply Trims**: Add/Subtract the digital trim values stored in ESP32 NVS.
3.  **Pack Frame**: Convert 16-bit values into the 24-byte buffer.
4.  **Bit-Bang RF**:
    - Iterate through the buffer bit-by-bit.
    - Change Si5351 CLK0 frequency for each bit.
    - Maintain precise timing using ESP32 High-Resolution Timers (`esp_timer`).
5.  **Listen for Telemetry**: Briefly switch the receiver (if implemented) or check for ACKs.

---

## 4. Receiver (RX) Logic Flow (SDR Mode)
Since we are using an ESP32-S3 as an SDR:
1.  **Sample IF**: Sample the 10.7MHz IF output from the mixer using the ESP32's high-speed ADC.
2.  **Software Demodulation**:
    - Use a digital Frequency Discriminator (comparing phase between samples).
    - Apply a low-pass filter to the demodulated signal.
3.  **Frame Sync**: Look for the `Sync Word`.
4.  **CRC Check**: If CRC passes, update the PWM outputs for the servos.
5.  **Failsafe**: If no valid packet is received for 500ms, move servos to neutral/preset.

---

## 5. Frequency Hopping (FHSS)
To comply with Ham Radio best practices and avoid interference:
- Define a table of 16 frequencies (e.g., 50.1, 50.2, ... 50.8 MHz).
- Both TX and RX hop to the next frequency every 100ms.
- Hopping sequence is determined by the 32-bit `Transmitter ID`.

---
*Created for the Kraft-6M Project.*
