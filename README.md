# 6-Meter Ham Band RC Transceiver Project

This project focuses on designing and building a modern 50MHz (6-meter) radio control system using the amateur radio bands. It features a high-power 1W transmitter and a high-sensitivity receiver, integrated into a vintage Kraft 7 transmitter case with a modern digital encoder.

## 📁 Repository Structure

### 📡 TX Hardware
- [PA_Design_Details.md](file:///TX_Hardware/PA_Design_Details.md): 1W RD01MUS2 Power Amplifier design and filtering.
- [Digital_Encoder_Conversion.md](file:///TX_Hardware/Digital_Encoder_Conversion.md): Strategy for upgrading the Kraft 7 to digital.
- [Kraft_Digital_UI_Wiring.md](file:///TX_Hardware/Kraft_Digital_UI_Wiring.md): Pinout for ESP32, ADS1115, and OLED.
- [TX_Schematic_Blueprint.md](file:///TX_Hardware/TX_Schematic_Blueprint.md): Pin-to-pin wiring for KiCad TX entry.
- [TX_BOM.md](file:///TX_Hardware/TX_BOM.md): Parts list for building the transmitter.
- [Analog_Wiring_Harness.md](file:///TX_Hardware/Analog_Wiring_Harness.md): Physical wiring for Kraft sticks and auxiliary pots.
- [Case_Modification_Guide.md](file:///TX_Hardware/Case_Modification_Guide.md): Cutting, mounting, and mechanical integration for the Kraft case.
- [Battery_Management_Design.md](file:///TX_Hardware/Battery_Management_Design.md): LiPo conversion and USB-C charging for the Kraft case.

### 🛩️ RX Hardware
- [RX_Design_Details.md](file:///RX_Hardware/RX_Design_Details.md): Front-end LNA, Mixer, and 10.7MHz IF stage.
- [Receiver_Servo_Design.md](file:///RX_Hardware/Receiver_Servo_Design.md): Servo bus wiring and MCU integration.
- [RX_PCB_Design_Guide.md](file:///RX_Hardware/RX_PCB_Design_Guide.md): Physical layout, grounding, and stackup for the RX board.
- [RX_Schematic_Blueprint.md](file:///RX_Hardware/RX_Schematic_Blueprint.md): Pin-to-pin wiring for KiCad schematic entry.
- [RX_Power_Supply_Design.md](file:///RX_Hardware/RX_Power_Supply_Design.md): Servo bus filtering and 3.3V LDO design.
- [RX_BOM.md](file:///RX_Hardware/RX_BOM.md): Parts list for building the receiver.

### 💻 Software
- [Kraft_Digital_Encoder_Example.ino](file:///Software/Kraft_Digital_Encoder_Example.ino): Arduino/C++ code for the transmitter.
- [Kraft_7_Firmware_Skeleton.ino](file:///Software/Kraft_7_Firmware_Skeleton.ino): Full code structure for dual ADC and Si5351 handler.
- [Firmware_Implementation.md](file:///Software/Firmware_Implementation.md): GFSK modulation and SDR logic.
- [Trim_Implementation.md](file:///Software/Trim_Implementation.md): Digital trim logic and math.
- [Project_Recommendations.md](file:///Software/Project_Recommendations.md): ELRS and SimpleTX project guidance.
- [Telemetry_Protocol_Design.md](file:///Software/Telemetry_Protocol_Design.md): 2-way 50MHz data packet definition.

### 📜 System Documentation
- [System_Requirements.md](file:///System_Docs/System_Requirements.md): High-level project specifications and architectural goals.
- [6m_ham_rc_transceiver_design.md](file:///System_Docs/6m_ham_rc_transceiver_design.md): System architecture and block diagrams.
- [Initial_BOM.md](file:///System_Docs/Initial_BOM.md): Complete parts list for ordering.
- [PCB_Layout_Strategy.md](file:///System_Docs/PCB_Layout_Strategy.md): RF grounding and isolation rules.
- [PCB_Footprint_Guide.md](file:///System_Docs/PCB_Footprint_Guide.md): SMT footprints and assembly instructions.
- [RF_Range_Analysis.md](file:///System_Docs/RF_Range_Analysis.md): Theoretical range and link budget for 1W on 50MHz.
- **KiCad Projects**:
    - [RX_50MHz_SDR.net](file:///KiCad_Projects/RX_50MHz_SDR/RX_50MHz_SDR.net): Ready-to-import Netlist for Receiver.
    - [TX_50MHz_1W.net](file:///KiCad_Projects/TX_50MHz_1W/TX_50MHz_1W.net): Ready-to-import Netlist for Transmitter.
    - [KiCad_Import_Instructions.md](file:///KiCad_Projects/KiCad_Import_Instructions.md): Step-by-step guide to building the boards.

---

## 🛠️ Project Goals
1.  **2-Way Communication**: Telemetry feedback (Battery, RSSI) from plane to transmitter.
2.  **High Power**: 1W output for superior range and reliability on the 50MHz band.
3.  **Modern Digital Link**: Moving away from old analog PPM to a robust GFSK digital protocol (ELRS-inspired).
4.  **Vintage Aesthetic**: Preserving the classic feel of the Kraft 7 transmitter while providing modern performance.

---
*Maintained by ne9n.*
