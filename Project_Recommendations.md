# Recommended Open Source Projects for Kraft 7 Digital

To accelerate the development of the Kraft 7 restomod, it is recommended to draw from these established open-source projects.

## 1. Encoder Project: SimpleTX
**Goal**: Handle stick reading, OLED menus, and model memory.

SimpleTX is an Arduino-based framework for DIY radio transmitters. It is highly modular and suitable for the ESP32.
- **Repository**: [https://github.com/m-reuter/SimpleTX](https://github.com/m-reuter/SimpleTX)
- **Key Features for Kraft**:
    - ADS1115 ADC support.
    - Multi-model memory.
    - Graphical OLED status screens.
    - Calibration routines for old potentiometers.

## 2. Link Project: ExpressLRS (ELRS)
**Goal**: Provide a robust, long-range digital link at 50MHz.

ExpressLRS is the industry standard for high-performance RC links. While primarily used at 2.4GHz/915MHz, the **Protocol Logic** and **CRSF Serial Format** are perfect for this project.
- **Repository**: [https://github.com/ExpressLRS/ExpressLRS](https://github.com/ExpressLRS/ExpressLRS)
- **Application for 50MHz**:
    - Use the **CRSF Protocol** to pass data between the Encoder task and the RF task.
    - Adapt the ELRS **Packet Framing** (Preamble, Sync, Payload, CRC) for the Si5351A FSK modulation.
    - Leverage ELRS **Telemetry** code to display airplane battery voltage on the Kraft's OLED.

## 3. Recommended Workflow
1. **Fork SimpleTX**: Adapt it to run on the ESP32-S3 and read the 16-bit ADS1115.
2. **Integrate CRSF**: Configure SimpleTX to output a 420kbaud CRSF serial stream internally.
3. **RF Driver**: Write a dedicated task that listens to the CRSF stream and bit-bangs the Si5351A frequency registers at 50.8MHz.

---
*Reference: Open source RC development community.*
