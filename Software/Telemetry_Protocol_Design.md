# 50MHz RC Telemetry Protocol Design

This document defines the 2-way data packet structure for sending airplane status (telemetry) back to the Kraft transmitter over the 50MHz link.

## 1. Physical Layer (PHY)
- **Modulation**: GFSK (Gaussian Frequency Shift Keying).
- **Data Rate**: 50 kbps (Balanced for range vs latency).
- **Frequency**: 50.0 - 54.0 MHz (User selectable).
- **Duplex**: Time Division (TDD). 90% TX (Control), 10% RX (Telemetry).

---

## 2. Packet Structure

Each telemetry packet is **12 bytes** long to keep the transmission window short.

| Byte | Field | Description | Range |
| :--- | :--- | :--- | :--- |
| 0 | **Sync** | Sync Word (0x99) | Fixed |
| 1 | **Device ID** | Unique ID for the Receiver | 0-255 |
| 2-3 | **Batt_V** | Main Flight Battery Voltage (mV) | 0 - 65535 mV |
| 4-5 | **Current** | Instantaneous Current (mA) | 0 - 65535 mA |
| 6 | **RSSI** | Signal Strength at Airplane | -128 to 0 dBm |
| 7 | **Link Quality**| Packet success rate | 0 - 100% |
| 8-9 | **Altitude** | Barometric or GPS altitude (m) | Signed Integer |
| 10 | **Flags** | Alarm bits (Low Batt, Failsafe) | Bitmask |
| 11 | **CRC8** | Error Checking (Dallas/Maxim) | Calculated |

---

## 3. Firmware Logic (ESP32-S3)

### 3.1 On the Airplane (RX)
Every 100ms, the receiver stops listening for control pulses, switches the Si5351A to the Telemetry frequency, and bursts the 12-byte packet.

### 3.2 On the Transmitter (TX)
The transmitter listens in the "gap" between its control frames. If a valid telemetry packet is received:
1.  **CRC Check**: Validate packet integrity.
2.  **Display Update**: Push the `Batt_V` and `RSSI` to the SH1106 OLED display.
3.  **Haptic/Audio**: Trigger a beep if `Flags` bit 0 (Low Battery) is set.

---

## 4. Telemetry Alarm Thresholds

| Sensor | Warning (Beep) | Critical (Continuous) |
| :--- | :--- | :--- |
| **LiPo 3S** | 10.8V (3.6V/cell) | 10.2V (3.4V/cell) |
| **RSSI** | -95 dBm | -105 dBm (Risk of Failsafe) |
| **Link Quality**| < 70% | < 50% |

---

## 5. Next Steps
- Implement the `CRC8` calculation function in the ESP32 code.
- Add an **INA219 I2C Current Sensor** to the receiver BOM for high-accuracy current/voltage sensing.

---
*Reference: ExpressLRS (ELRS) Telemetry Format & CRSF Protocol.*
