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
| **Active Mixer** | LT5560EDD#PBF | Active Mixer (50MHz -> 10.7MHz) | 1 | Down-converter |
| **IF Filter** | 10.7 MHz | Ceramic Filter (230kHz BW, Murata SFE) | 1 | Selectivity |

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
| **Decoupling Caps**| 0.1 uF | 0603 X7R (Digital Bypassing) | 10 | |

## 4. Total Cost Estimate
- **Core Silicon**: ~$22.00
- **Passives & Connectors**: ~$8.00
- **Total per RX**: **~$30.00 USD** (excluding PCB)

---
*Note: Prices are approximate and based on single-unit quantities as of 2024.*
