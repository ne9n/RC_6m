# PCB Footprint & Assembly Guide

This guide provides the specific SMT footprint requirements and soldering advice for the 50MHz RC project.

## 1. Core Component Footprints

| Component | Function | Footprint Name | Dimensions / Pitch | Note |
| :--- | :--- | :--- | :--- | :--- |
| **ESP32-S3-WROOM-1** | MCU | `RF_Module:ESP32-S3-WROOM-1` | 1.27mm Pitch | Standard Espressif Footprint |
| **Si5351A-B-GT** | Synth | `Package_SO:MSOP-10_3x3mm_P0.5mm` | 0.5mm Pitch | Small, requires fine soldering |
| **ADS1115IDGS** | ADC | `Package_SO:VSSOP-10_3x3mm_P0.5mm` | 0.5mm Pitch | Same footprint as Si5351A |
| **RD01MUS2** | 1W PA | `Package_TO_SOT_SMD:SOT-89-3` | Thermal Tab | Solder center pad to GND plane |
| **LT5560EDD** | Mixer | `Package_DFN_QFN:DFN-8-1EP_3x3mm_P0.5mm`| 0.5mm Pitch | Exposed Pad (EP) is critical |
| **SPF5043Z** | LNA | `Package_TO_SOT_SMD:SOT-343` | 1.3mm / 0.65mm | RF Optimized pinout |
| **AP2112K** | LDO | `Package_TO_SOT_SMD:SOT-23-5` | 0.95mm Pitch | Standard regulator |

---

## 2. Passive Component Sizes
To balance ease of hand-soldering with RF performance:
- **0805 (2012 Metric)**: Recommended for Power Amplifiers (TX) and high-current paths.
- **0603 (1608 Metric)**: Recommended for all RF signals and general decoupling.
- **0402 (1005 Metric)**: Avoid unless you have a microscope; not needed for 50MHz.

---

## 3. Critical Solder Mask & Stencil Advice
- **Thermal Pads**: For the **RD01MUS2** and **LT5560**, the center thermal pad MUST be connected to the bottom ground plane using a cluster of 4-9 "Thermal Vias" (0.2mm - 0.3mm diameter).
- **RF Traces**: Do not allow solder mask to cover 50-ohm RF traces if you are doing professional impedance control; however, for 50MHz, standard mask is acceptable.
- **Solder Paste**: Use **Lead-Free SAC305** or **Leaded Sn63/Pb37** for easiest hand assembly.

---

## 4. Connector Footprints
- **Servo Headers**: `Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical`
- **SMA Connector**: `Connector_Coaxial:SMA_Amphenol_132203_Vertical`
- **I2C / OLED**: `Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical`

---
*Reference: IPC-7351B Standard for Surface Mount Design.*
