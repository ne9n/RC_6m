# Kraft-6M: KiCad PCB Footprint Mapping

This document maps the BOM components to the exact KiCad library footprint names to simplify the board layout process.

## 1. Transmitter (TX) Footprints

| Component | KiCad Library Footprint | Note |
| :--- | :--- | :--- |
| **ESP32-S3-WROOM-1** | `RF_Module:ESP32-S3-WROOM-1-N8` | Standard Espressif Module |
| **Si5351A-B-GT** | `Package_SO:MSOP-10_3x3mm_P0.5mm` | 10-pin MSOP |
| **RD01MUS2** | `Package_TO_SOT_SMD:SOT-89-3` | Center pad is RF Ground/Heat Sink |
| **ADS1115IDGSR** | `Package_SO:VSSOP-10_3x3mm_P0.5mm` | |
| **1.3" OLED (SH1106)** | `Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical` | 4-pin I2C header |
| **26MHz Crystal** | `Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm` | Standard 3.2x2.5mm SMD |
| **Inductors (0805)** | `Inductor_SMD:L_0805_2012Metric` | |
| **Capacitors (0805)** | `Capacitor_SMD:C_0805_2012Metric` | |
| **RF Choke (1206)** | `Inductor_SMD:L_1206_3216Metric` | |
| **SMA (Edge Mount)** | `Connector_Coaxial:SMA_Amphenol_132289_EdgeMount` | For 1.6mm PCB |
| **JST-XH (3-pin)** | `Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical` | |

---

## 2. Receiver (RX) Footprints

| Component | KiCad Library Footprint | Note |
| :--- | :--- | :--- |
| **ESP32-S3-WROOM-1** | `RF_Module:ESP32-S3-WROOM-1-N8` | |
| **LT5560 Mixer** | `Package_SON:DFN-8-1EP_2x2mm_P0.5mm_EP0.6x1.0mm` | 2x2mm DFN-8 |
| **SPF5043Z LNA** | `Package_TO_SOT_SMD:SOT-343_SC-70-4` | Or use `ul_SPF5043Z` custom lib |
| **IF Filter (10.7MHz)**| `Filter:Filter_Ceramic_Murata_SFE_7.0x7.0mm_P2.50mm` | 2.5mm Lead Pitch |
| **AP2112K-3.3** | `Package_TO_SOT_SMD:SOT-23-5` | |
| **Inductors (0603)** | `Inductor_SMD:L_0603_1608Metric` | Used for matching |
| **Capacitors (0603)** | `Capacitor_SMD:C_0603_1608Metric` | Used for RF path |
| **Bulk Cap (470uF)** | `Capacitor_THT:CP_Radial_D8.0mm_P3.50mm` | Electrolytic |
| **SS14 Diode** | `Diode_SMD:D_SMA` | |
| **Servo Headers** | `Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical` | |

---

## 3. General Passive Sizes
*   **0805 (2012 Metric)**: Used for TX Power/Filter stages for higher power handling.
*   **0603 (1608 Metric)**: Used for RX signal path to minimize parasitic inductance.
*   **C0G/NP0**: Ensure all RF capacitors (LPF/Matching) are C0G/NP0 dielectric for frequency stability.

*Created for the Kraft-6M Project.*
