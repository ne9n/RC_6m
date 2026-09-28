# 50MHz Receiver: Bill of Materials (BOM)

This list contains the core components required to build one 50MHz high-sensitivity receiver with integrated 2-way telemetry.

## 1. Core Integrated Circuits (Logic & Power)

| Component | Part Number | Description | Qty | Source |
| :--- | :--- | :--- | :--- | :--- |
| **MCU** | ESP32-S3-WROOM-1 | ESP32-S3 Module (Dual-core, 16MB Flash) | 1 | [DigiKey](https://www.digikey.com/en/products/detail/espressif-systems/ESP32-S3-WROOM-1-N16R8/15970929) |
| **Synthesizer** | Si5351A-B-GT | I2C Clock Generator (LO + Telemetry) | 1 | [DigiKey](https://www.digikey.com/en/products/detail/skyworks-solutions-inc/SI5351A-B-GT/3847847) |
| **Regulator** | AP2112K-3.3TRG1 | 3.3V 600mA LDO Regulator | 1 | [DigiKey](https://www.digikey.com/en/products/detail/diodes-incorporated/AP2112K-3-3TRG1/4470746) |
| **Current Sensor**| INA219AIDCNR | I2C Current/Power Monitor (Telemetry) | 1 | [DigiKey](https://www.digikey.com/en/products/detail/texas-instruments/INA219AIDCNR/2135017) |

## 2. RF Path Components

### 2.1 Receiver Stage
| Component | Part Number | Description | Qty | Note |
| :--- | :--- | :--- | :--- | :--- |
| **LNA** | SPF5043Z | Low Noise MMIC Amplifier (0.8dB NF) | 1 | Front-end |
| **Active Mixer** | LT5560EDD#PBF | Active Mixer (50MHz -> ~169MHz IF, LO 118.2MHz) | 1 | Up-converter |
| **IF Band-Pass** | ~168.75 MHz | 3-pole LC BPF, ~3-5MHz BW, 50 Ohm (values TBD) | 1 set | Rejects LO/RF feedthrough |
| **FSK Receiver** | Si4463-C2A-GM | EZRadioPRO sub-GHz transceiver, QFN-20 4x4mm, receive-only | 1 | Demod, AFC, sync, RSSI (SPI) |
| **Crystal (Si4463)** | 30.000 MHz | 3225, <=+-10 ppm preferred (internal load caps) | 1 | Si4463 reference |
| **Si4463 RX Match** | 169 MHz | Single-ended -> differential match per Silicon Labs AN643 (values TBD) | 1 set | |
| **Resistor** | 10 kOhm | 0603, Si4463 nIRQ pull-up | 1 | |

*Removed 2026-09-28:* 10.7MHz ceramic IF filter (Murata SFE) and the CD74HC4046A PLL
discriminator (+ its timing R/C, loop filter and demod RC LPF) -- replaced by the Si4463
upconversion receiver (see `RX_Design_Details.md`).

### 2.2 Telemetry Transmitter Stage
| Component | Part Number | Description | Qty | Note |
| :--- | :--- | :--- | :--- | :--- |
| **RF Switch** | BGS12PL6 | SPDT T/R Switch | 1 | Antenna Sharing |
| **Crystal** | 26.000 MHz | For Si5351A Reference | 1 | 8pF Load |

## 3. Passives & Connectors

| Component | Value | Description | Qty | Note |
| :--- | :--- | :--- | :--- | :--- |
| **Servo Headers** | 0.1" (2.54mm) | 3-pin Male Headers (Breakaway) | 8 | PWM Out |
| **Bulk Cap** | 470 uF | Electrolytic (Servo Rail Buffer) | 1 | |
| **LDO VCC** | 10 uF | 0805 X7R (LDO Input/Output) | 2 | |
| **RF Path Caps** | 100 pF | 0603 C0G/NP0 (RF Coupling) | 8 | Increased qty |
| **Inductor (Match)**| 220 nH | 0603 Wirewound (LNA Match) | 2 | 50MHz Tuned |
| **Inductor (RFC)** | 1.0 uH | 0805 Power Inductor (LNA Bias) | 1 | |
| **Protection** | SS14 | Schottky Diode (Reverse Polarity) | 1 | |
| **Power Filter** | 10 uH | 0805 Power Inductor (LC Filter) | 1 | |
| **Decoupling Caps**| 0.1 uF | 0603 X7R (Digital Bypassing) | 12 | +2 for Si4463 VDD |
| **Decoupling Caps**| 1 uF / 10 pF | 0402 X7R / C0G (Si4463 VDD pins 6, 8) | 2 / 1 | |

## 4. Total Cost Estimate
- **Core Silicon**: ~$22.00
- **Passives & Connectors**: ~$8.00
- **Total per RX**: **~$30.00 USD** (excluding PCB)

---
*Note: Prices are approximate and based on single-unit quantities as of 2024.*
