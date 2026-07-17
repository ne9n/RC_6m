# Initial Bill of Materials (BOM) - 6m RC Transceiver

This is the initial parts list for the 50MHz digital RC link. It uses modern, active components available through major distributors (DigiKey, Mouser, etc.).

## 1. Primary Components (RF & Logic)

| RefDes | Part Number | Manufacturer | Description | Approx. Price |
| :--- | :--- | :--- | :--- | :--- |
| U1 | [Si5351A-B-GT](https://www.digikey.com/en/products/detail/skyworks-solutions-inc/SI5351A-B-GT/3679815) | Skyworks | I2C Programmable Clock Generator (Synthesizer) | $2.50 |
| U2 | [RD01MUS2-T113](https://www.rfparts.com/rd01mus2-t113.html) | Mitsubishi | 1W VHF/UHF RF MOSFET (PA) | $8.00 |
| U3 | [LT5560EDD#PBF](https://www.digikey.com/en/products/detail/analog-devices-inc/LT5560EDD-PBF/1590059) | Analog Devices | Low Power Active Mixer (40MHz - 4GHz) | $4.50 |
| U4 | [SPF5043Z](https://www.digikey.com/en/products/detail/qorvo/SPF5043Z/4836657) | Qorvo | Low Noise MMIC Amplifier (LNA) | $2.00 |
| U5 | [BGS12PL6E6327XTSA1](https://www.digikey.com/en/products/detail/infineon-technologies/BGS12PL6E6327XTSA1/2353386) | Infineon | SPDT RF Switch | $0.80 |
| U6 | [ESP32-S3-WROOM-1](https://www.digikey.com/en/products/detail/espressif-systems/ESP32-S3-WROOM-1-N8/15970878) | Espressif | Ground Station MCU (WiFi/BT) | $4.00 |
| U7 | [ESP32-S3-WROOM-1](https://www.digikey.com/en/products/detail/espressif-systems/ESP32-S3-WROOM-1-N8/15970878) | Espressif | Airplane RX MCU (Unified with TX) | $4.00 |
| U8 | [INA219AIDCNR](https://www.digikey.com/en/products/detail/texas-instruments/INA219AIDCNR/2135017) | TI | I2C Current/Power Monitor (Telemetry) | $2.50 |
| Y1 | [ECS-260-18-33-JGN-TR](https://www.digikey.com/en/products/detail/ecs-inc/ECS-260-18-33-JGN-TR/825413) | ECS Inc. | 26MHz TCXO / Crystal (Frequency Ref) | $1.00 |

## 2. Filters & Passives

| Component | Value / Type | Note |
| :--- | :--- | :--- |
| **IF Filter** | 10.7 MHz | Ceramic Filter (Standard SMT or THT) |
| **LPF Inductors** | 120nH / 150nH | Wire-wound 0603 High-Q (Murata LQW series) |
| **PA RFC** | 1.0 uH | High-current inductor (400mA+) for DC feed |
| **Matching Caps** | C0G/NP0 | RF Grade capacitors (e.g., Murata GJM series) |

## 3. Connectors & Misc

| Component | Description |
| :--- | :--- |
| **Antenna (TX)** | 1.5m Telescoping Whip (BNC or SMA mount) |
| **Antenna (RX)** | 26AWG Silicone Wire (Trailing 1.5m) |
| **PCB (Air)** | 20x20mm 4-Layer (Recommended for RF performance) |
| **PCB (Ground)** | JR Module form factor (Standard RC expansion bay) |

---

### Total Estimated Prototype Cost (Silicon Only): ~$35.00 USD

> [!NOTE]
> The **LT5560** is used in this BOM as a modern, high-performance replacement for the obsolete SA612. It requires an external LO from the Si5351A but offers significantly better dynamic range and lower noise.
