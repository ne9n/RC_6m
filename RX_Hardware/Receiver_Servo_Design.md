# 6m RC Receiver: Back-End & Servo Connections

This document details the hardware and software required to turn the 50MHz RF signal into physical control for your airplane's servos.

## 1. Receiver Architecture (Back-End)

The RF deck upconverts the 50MHz signal to a ~169MHz IF and an **Si4463** FSK receiver demodulates it in hardware; the MCU (ESP32-S3) reads decoded packets over SPI (see `RX_Design_Details.md`). The MCU then generates **PWM (Pulse Width Modulation)** signals for the servos.

### 1.1 MCU Choice: ESP32-S3
The ESP32-S3 is recommended for the receiver because:
- **High Speed**: Dual-core 240MHz is plenty for packet handling, hopping and servo output.
- **PWM Peripherals**: The `MCPWM` (Motor Control PWM) hardware is perfect for generating jitter-free servo signals.
- **Tiny Form Factor**: Available in small modules (ESP32-S3-WROOM-1) suitable for aircraft.

---

## 2. Servo Connection Schematic

Standard RC servos use a 3-pin connector: **GND, 5V (VCC), and Signal (PWM)**.

### 2.1 Wiring Diagram
```mermaid
graph TD
    subgraph "50MHz RF Deck"
        RF["LT5560 Upconverter + 169MHz IF + Si4463"]
    end

    subgraph "Receiver Brain (ESP32-S3)"
        ADC["SPI (Si4463 packets)"]
        MCU["ESP32-S3 Core"]
        PWM["PWM Outputs (Ch 1-8)"]
    end

    subgraph "Servo Bus (Power Rail)"
        BEC["ESC / BEC (5V Input)"]
        S1["Servo 1 (Ail)"]
        S2["Servo 2 (Ele)"]
        S3["Servo 3 (Thr)"]
        S4["Servo 4 (Rud)"]
    end

    RF -- "Decoded packets" --> ADC
    ADC --> MCU
    MCU --> PWM
    PWM -- "Signal" --> S1 & S2 & S3 & S4
    BEC -- "5V Rail" --> S1 & S2 & S3 & S4
```

### 2.2 Servo Pin Mapping (ESP32)
| Channel | Function | ESP32 Pin | Note |
| :--- | :--- | :--- | :--- |
| **CH 1** | Aileron | GPIO 4 | 50Hz PWM, 1000-2000us (the old `ADC_IF` net on IO4 is removed with the Si4463 redesign, clearing this clash) |
| **CH 2** | Elevator | GPIO 5 | 50Hz PWM |
| **CH 3** | Throttle | GPIO 6 | Connects to ESC Signal |
| **CH 4** | Rudder | GPIO 7 | 50Hz PWM |
| **CH 5** | Aux 1 | GPIO 15 | Gear / Flaps |
| **CH 6** | Aux 2 | GPIO 16 | |
| **CH 7** | Aux 3 | GPIO 1 | |
| **CH 8** | Aux 4 | GPIO 2 | |

---

## 3. Power Management (The Servo Bus)

**CRITICAL**: Servos draw significant current (up to 2A during stalls). Do **NOT** power servos directly from the ESP32's 3.3V regulator.

1.  **Common Rail**: Create a high-current 5V rail (the "Servo Bus") on your PCB.
2.  **BEC Input**: Use the 5V output from your Electronic Speed Controller (ESC) to power this rail.
3.  **Isolation**: Add a large capacitor (470uF - 1000uF) on the 5V rail to prevent voltage dips (Brown-outs) when servos move rapidly, which could reset the ESP32.

---

## 4. PWM Firmware (ESP32 C++ Snippet)

Using the ESP32 `ESP32Servo` library for easy implementation.

```cpp
#include <ESP32Servo.h>

Servo aileron;
Servo elevator;

void setup() {
    // Standard RC PWM: 50Hz, 1000us min, 2000us max
    aileron.attach(4, 1000, 2000); 
    elevator.attach(5, 1000, 2000);
}

void updateServos(int ail_val, int ele_val) {
    // ail_val and ele_val are decoded from the 50MHz radio link
    aileron.writeMicroseconds(ail_val);
    elevator.writeMicroseconds(ele_val);
}
```

---

## 5. Physical Layout Recommendation
- **Servo Headers**: Use standard 3-pin 0.1" (2.54mm) male headers arranged in a row.
- **Antenna**: A 1.5m thin wire (28AWG) trailing from the plane. Ensure it is routed away from the servo wires to minimize interference.

---
*Created for the 50MHz Ham Band RC Project.*
