# 50MHz Receiver: Bill of Materials (BOM)

This list contains the core components required to build one 50MHz high-sensitivity receiver.

## 1. Core Integrated Circuits

| Component | Part Number | Description | Qty | Source |
| :--- | :--- | :--- | :--- | :--- |
| **MCU** | ESP32-S3-WROOM-1 | ESP32-S3 Module (Dual-core, 16MB Flash) | 1 | [DigiKey](https://www.digikey.com/en/products/detail/espressif-systems/ESP32-S3-WROOM-1-N16R8/15970929) |
| **Active Mixer** | LT5560EDD#PBF | Active Mixer (Modern SA612 replacement) | 1 | [DigiKey](https://www.digikey.com/en/products/detail/analog-devices-inc/LT5560EDD-PBF/1590059) |
| **LNA** | SPF5043Z | Low Noise MMIC Amplifier (0.8dB NF) | 1 | [DigiKey](https://www.digikey.com/en/products/detail/qorvo/SPF5043Z/4694406) |
| **Synthesizer** | Si5351A-B-GT | I2C Clock Generator (Local Oscillator) | 1 | [DigiKey](https://www.digikey.com/en/products/detail/skyworks-solutions-inc/SI5351A-B-GT/3847847) |
| **Regulator** | AP2112K-3.3TRG1 | 3.3V 600mA LDO Regulator | 1 | [DigiKey](https://www.digikey.com/en/products/detail/diodes-incorporated/AP2112K-3-3TRG1/4470746) |

## 2. RF & Frequency Components

| Component | Value | Description | Qty | Note |
| :--- | :--- | :--- | :--- | :--- |
| **IF Filter** | 10.7 MHz | Ceramic Filter (230kHz BW, Murata SFE) | 1 | [Mouser](https://www.mouser.com/Search/Refine?Keyword=10.7MHz+ceramic+filter) |
| **Crystal** | 26.000 MHz | For Si5351A (8pF Load) | 1 | [DigiKey](https://www.digikey.com/en/products/detail/txc-corporation/8Z26070012/5691963) |
| **Inductor (Match)**| 220 nH | 0603 Wirewound (LNA Input Match) | 2 | 50MHz Tuned |
| **Inductor (RFC)** | 1.0 uH | 0805 Power Inductor (LNA Bias) | 1 | |

## 3. Connectors & Passives

| Component | Value | Description | Qty |
| :--- | :--- | :--- | :--- |
| **Servo Headers** | 0.1" (2.54mm) | 3-pin Male Headers (Breakaway) | 8 |
| **Bulk Cap** | 470 uF | Electrolytic (Servo Rail Buffer) | 1 |
| **RF Capacitors** | 100 pF | 0603 C0G/NP0 (RF Coupling) | 5 |
| **Decoupling Caps**| 0.1 uF | 0603 X7R (Digital Bypassing) | 10 |

## 4. Total Cost Estimate
- **Core Silicon**: ~$18.00
- **Passives & Connectors**: ~$5.00
- **Total per RX**: **~$23.00 USD** (excluding PCB)

---
*Note: Prices are approximate and based on single-unit quantities as of 2024.*
