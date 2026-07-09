# Kraft Digital: Wiring Schematic & OLED UI Design

This document details the hardware interconnections and the user interface design for the Kraft 7 Digital Conversion.

## 1. Wiring Schematic

The system uses a shared **I2C Bus** (SDA/SCL) to communicate between the ESP32, the high-precision ADC, the OLED display, and the RF Synthesizer.

### 1.1 Pin Mapping (ESP32-S3)

| Peripheral | Pin | Connection | Note |
| :--- | :--- | :--- | :--- |
| **I2C SDA** | GPIO 8 | ADS1115, OLED, Si5351 | Shared Data Bus |
| **I2C SCL** | GPIO 9 | ADS1115, OLED, Si5351 | Shared Clock Bus |
| **Aux SW 1** | GPIO 1 | Toggle Switch | Dual Rates / Mode |
| **Aux SW 2** | GPIO 2 | Toggle Switch | Gear / Aux |
| **Aux Pot 1** | GPIO 3 | Center Wiper | Analog Aux Channel (Internal ADC) |
| **RF EN** | GPIO 10 | PA Bias Enable | Enables the 1W Transmitter |
| **Status LED** | GPIO 21 | WS2812B (Built-in) | System Health / Link Status |

### 1.2 ADC Wiring (ADS1115 - High Precision)

To ensure smooth, "jitter-free" control, the four primary flight axes are connected to the 16-bit ADS1115.

| Stick Axis | Kraft Pot terminal | ADS1115 Pin |
| :--- | :--- | :--- |
| **Aileron** | Center Wiper | A0 |
| **Elevator** | Center Wiper | A1 |
| **Throttle** | Center Wiper | A2 |
| **Rudder** | Center Wiper | A3 |

> [!IMPORTANT]
> **Voltage Reference**: Connect the top terminal of all Kraft potentiometers to the ESP32 **3.3V** rail and the bottom terminal to **GND**. This ensures the stick range is within the ADC's safe operating window.

---

## 2. OLED UI Design (1.3" SH1106)

The display is divided into three logical zones: Status, Flight Data, and Telemetry.

### 2.1 Screen Layout Mockup

```text
+------------------------------------------+
| NE9N-TX      [8.4V]      LQ: 100% (RSSI) | <--- Status Bar
+------------------------------------------+
|                                          |
|    THR: [|||||     ]    AIL: ---|---     |
|    RUD: [|||||||   ]    ELE: ---|---     | <--- Stick Monitor
|                                          |
+------------------------------------------+
| PLANE: 12.6V | TIMER: 05:42 | RATE: HIGH | <--- Telemetry Bar
+------------------------------------------+
```

### 2.2 Visual Elements
- **Callsign (Top Left)**: Required for Amateur Radio digital identification.
- **LQ (Link Quality)**: Percentage of successful telemetry packets received back from the plane.
- **Stick Monitor**:
  - **Throttle/Rudder**: Horizontal progress bars.
  - **Ail/Ele**: Small "Crosshair" displays representing the gimbal position.
- **PLANE (Bottom Left)**: Real-time battery voltage of the airplane (via 50MHz telemetry).
- **RATE (Bottom Right)**: Indicates if the transmitter is in High or Low rates (Dual Rates).

---

## 3. Hardware Assembly Tips

1. **Shielded Cables**: Use shielded "servo wire" for the connections between the Kraft stick pots and the ADS1115. At 1W transmit power, RF interference can easily leak into the analog stick signals.
2. **I2C Pull-ups**: Ensure you have 2.2k or 4.7k pull-up resistors on the SDA/SCL lines. With three devices on the bus, the internal ESP32 pull-ups are usually insufficient.
3. **Mechanical Mounting**: Use the original Kraft RF deck mounting holes to secure a 3D-printed bracket for the ESP32 and RF modules.

---
*Created for the Kraft 7 "Restomod" Project.*
