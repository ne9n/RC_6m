# Kraft-6M: Firmware Implementation & Protocol Guide

This document details the software logic required to run the 50MHz digital RC link using the ESP32-S3 and Si5351A.

## 1. Modulation Logic (Si5351A GFSK)
Unlike 2.4GHz chips (LoRa/CC2500), the Si5351A is a clock generator. To perform **Gaussian Frequency Shift Keying (GFSK)**, we must "pull" the frequency of the oscillator in real-time.

### 1.1 Bit Representation
- **Center Frequency ($f_c$):** 50.100 MHz
- **Frequency Deviation ($\Delta f$):** ±5 kHz (project standard -- older docs/code used ±2.4 kHz; the Si4463 receivers are configured for ±5 kHz, so the TX must match)
- **Logic '1':** $f_c + 5$ kHz
- **Logic '0':** $f_c - 5$ kHz

### 1.2 Implementation
We use the Si5351's **VCXO (Voltage Controlled Crystal Oscillator)** capability or rapidly update the `MultiSynth` registers via I2C.
- **Data Rate:** 2400 bps or 4800 bps (to keep bandwidth narrow and range high).
- **Encoding:** **NRZ, no whitening** (bring-up default). The Si4463 receiver can't decode 8b/10b, so that option is dropped; Manchester is supported in Si4463 hardware and is the fallback if long runs cause bit errors (doubles the on-air symbol rate).
- **Preamble:** at least **5 bytes (40 bits) of alternating `0x55`** ahead of the sync word, so the Si4463's AFC and preamble detector settle.
- **Separate PLLs:** the Si5351A's CLK0 (FSK carrier) and CLK1 (118.2MHz receive LO) sit on PLLA and PLLB, so FSK updates never disturb the LO.

---

## 2. Packet Structure (24 Bytes)
Each frame transmitted every 20ms (50Hz) follows this structure:

| Byte | Field | Description |
| :--- | :--- | :--- |
| 0-1 | **Sync Word** | 0x1D4A (detected by the Si4463 packet handler; bytes 2-23 are the fixed-length 22-byte payload) |
| 2-17| **Channels** | 8 Channels x 16-bits (from ADS1115) |
| 18 | **Flags** | Failsafe bit, Telemetry request bit |
| 19-22| **ID** | 32-bit Unique Transmitter ID |
| 23 | **CRC8** | Cyclic Redundancy Check (Polynomial: 0x07) -- computed/checked in firmware; Si4463 hardware CRC disabled |

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

## 4. Receiver (RX) Logic Flow (Si4463)
The 6m signal is upconverted to ~169MHz (LT5560 + fixed 118.2MHz Si5351A LO) and demodulated by an **Si4463**; the ESP32-S3 reads finished packets over SPI. (The earlier SDR/IF-sampling and 4046 PLL-discriminator flows are superseded -- see `Firmware_Implementation.md`.)
1.  **Init**: SDN low, wait for POR, load the WDS/Radio Configurator config (`radio_config_si4463.h`) over SPI, polling CTS. AFC enabled.
2.  **Listen**: `START_RX` on the current hop channel, fixed packet length.
3.  **Packet**: on nIRQ (PACKET_RX), read the 22-byte payload from the RX FIFO and the latched RSSI.
4.  **CRC Check**: If the firmware CRC8 passes, update the PWM outputs for the servos.
5.  **Failsafe**: If no valid packet is received for 500ms, move servos to neutral/preset.

---

## 5. Frequency Hopping (FHSS)
To comply with Ham Radio best practices and avoid interference:
- Define a table of 16 frequencies (e.g., 50.1, 50.2, ... 50.8 MHz).
- Both TX and RX hop to the next frequency every 100ms.
- **Receive side**: hopping is done by the **Si4463 channel number** (`START_RX` on the next channel; channel step = hop spacing). The 118.2MHz LO stays fixed -- no Si5351A retune or I2C traffic on the receive side per hop. The transmit side still retunes Si5351A CLK0.
- **Note**: the example table (50.1-50.8 MHz) overlaps the 6m weak-signal CW/SSB segment (50.0-50.3 MHz). The US RC channels are 50.800-50.980 MHz -- review the hop table before use.
- Hopping sequence is determined by the 32-bit `Transmitter ID`.

---
*Created for the Kraft-6M Project.*
