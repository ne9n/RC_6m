# 50MHz Transmitter: Bill of Materials (BOM)

This list contains the core components for the 1W Kraft 7 digital conversion.

## 1. Core Integrated Circuits

| Component | Part Number | Description | Qty | Source |
| :--- | :--- | :--- | :--- | :--- |
| **MCU** | ESP32-S3-WROOM-1 | ESP32-S3 Module (Dual-core, 16MB) | 1 | [DigiKey](https://www.digikey.com/en/products/detail/espressif-systems/ESP32-S3-WROOM-1-N16R8/15970929) |
| **RF Synthesizer** | Si5351A-B-GT | I2C Clock Generator | 1 | [DigiKey](https://www.digikey.com/en/products/detail/skyworks-solutions-inc/SI5351A-B-GT/3847847) |
| **Power MOSFET** | RD01MUS2-T113 | 1W RF Power Transistor (SOT-89) | 1 | [RF Parts](https://www.rfparts.com/rd01mus2-t113.html) |
| **Precision ADC** | ADS1115IDGSR | 16-bit 4-Ch I2C ADC | 1 | [DigiKey](https://www.digikey.com/en/products/detail/texas-instruments/ADS1115IDGSR/2060195) |
| **Display** | 1.3" OLED (SH1106) | I2C 128x64 Monochrome Display | 1 | [Amazon/eBay](https://www.amazon.com/s?k=1.3+inch+OLED+SH1106) |

## 2. RF & Passive Components

| Component | Value | Description | Qty |
| :--- | :--- | :--- | :--- |
| **Crystal** | 26.000 MHz | For Si5351A (8pF Load) | 1 |
| **Inductor (LPF)** | 180 nH | 0805 Wirewound (7-pole filter) | 3 |
| **Capacitor (LPF)** | 220 pF | 0805 C0G/NP0 (7-pole filter) | 2 |
| **Capacitor (LPF)** | 120 pF | 0805 C0G/NP0 (7-pole filter) | 2 |
| **RF Choke** | 1.0 uH | 1206 High-Current Inductor (PA Feed) | 1 |

## 3. Power & Mechanical

| Component | Part Number | Description | Qty |
| :--- | :--- | :--- | :--- |
| **Buck Converter** | MP1584EN | 7-12V to 5V Step Down Module | 1 |
| **SMA Connector** | SMA-KWE | Right Angle PCB Mount SMA | 1 |
| **Antenna** | 50MHz Whip | Base-loaded or telescopic whip | 1 |

## 4. Total Cost Estimate
- **Core Silicon**: ~$25.00
- **Passives & Display**: ~$15.00
- **Total per TX**: **~$40.00 USD**

---
*Note: The RD01MUS2 is a specialized RF part and may require ordering from specialized vendors like RF Parts or Mouser.*
