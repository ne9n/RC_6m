# Kraft-6M: KiCad PCB Footprint Mapping

This document maps the BOM components to the exact KiCad library footprint names to simplify the board layout process.

## 1. Transmitter (TX) Footprints

| Component | KiCad Library Footprint | Note |
| :--- | :--- | :--- |
| **ESP32-S3-WROOM-1** | `RF_Module:ESP32-S3-WROOM-1-N8` | Standard Espressif Module |
| **Si5351A-B-GT** | `Package_SO:MSOP-10_3x3mm_P0.5mm` | 10-pin MSOP |
| **RD01MUS2** | `Kraft6M:RD01MUS2` | Center pad is RF Ground/Heat Sink |
| **ADS1115IDGSR** | `Kraft6M:ADS1115` | |
| **1.3" OLED (SH1106)** | `Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical` | 4-pin I2C header |
| **26MHz Crystal** | `Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm` | Standard 3.2x2.5mm SMD |
| **Inductors (180nH)** | `Kraft6M:L_180nH_0805` | Specific for 50MHz LPF |
| **Capacitors (120/220pF)**| `Kraft6M:C_120pF_0805` / `C_220pF_0805` | Specific for 50MHz LPF |
| **RF Choke (1.0uH)** | `Kraft6M:L_1.0uH_1206` | High-current bias tee |
| **SMA (Edge Mount)** | `Connector_Coaxial:SMA_Amphenol_132289_EdgeMount` | For 1.6mm PCB |
| **JST-XH (3-pin)** | `Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical` | |

---

## 2. Receiver (RX) Footprints

| Component | KiCad Library Footprint | Note |
| :--- | :--- | :--- |
| **ESP32-S3-WROOM-1** | `RF_Module:ESP32-S3-WROOM-1-N8` | |
| **LT5560 Mixer** | `Kraft6M:LT5560` | 2x2mm DFN-8 |
| **SPF5043Z LNA** | `Kraft6M:SPF5043Z` | |
| **IF Filter (10.7MHz)**| `Kraft6M:SFE10.7` | Murata SFE Series |
| **AP2112K-3.3** | `Package_TO_SOT_SMD:SOT-23-5` | |
| **Inductors (Match)** | `Kraft6M:L_220nH_0603` | Specific for LNA match |
| **Capacitors (Match)** | `Kraft6M:C_100pF_0603` | Specific for RF path |
| **Bulk Cap (470uF)** | `Kraft6M:C_470uF_Radial` | Electrolytic |
| **SS14 Diode** | `Diode_SMD:D_SMA` | |
| **Servo Headers** | `Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical` | |

---

## 3. General Passive Sizes
*   **0805 (2012 Metric)**: Used for TX Power/Filter stages for higher power handling.
*   **0603 (1608 Metric)**: Used for RX signal path to minimize parasitic inductance.
*   **C0G/NP0**: Ensure all RF capacitors (LPF/Matching) are C0G/NP0 dielectric for frequency stability.

*Created for the Kraft-6M Project.*
