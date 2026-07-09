# RD01MUS1 1W Power Amplifier Design (50MHz)

This document provides a detailed technical description of the **1 Watt RF Power Amplifier (PA)** stage for the 6m (50MHz) ground station transceiver.

## 1. Core Component: RD01MUS1
The **RD01MUS1** is a Silicon MOSFET designed for VHF/UHF power amplification.
- **Package**: SOT-89 (Small Outline Transistor).
- **Drain-Source Voltage ($V_{DSS}$)**: 30V Max.
- **Output Power ($P_{out}$)**: Typical 0.8W - 1.2W at 7.2V.
- **Gain**: ~14dB at 50MHz (High gain, requires stable biasing).

---

## 2. Schematic Topology

The PA follows a standard Class AB configuration for a balance between linearity (important for GFSK) and efficiency.

### 2.1 Input Matching Network
The input impedance of the RD01MUS1 at 50MHz is low and primarily capacitive.
- **Topology**: L-Match or a 4:1 Broadband Transformer (Binocular Core).
- **Components**: 
    - Series Inductor: ~47nH.
    - Shunt Capacitor: ~82pF.
- **Goal**: Transform the 50 Ohm input from the Si5351A/Buffer to the ~5-10 Ohm gate impedance.

### 2.2 Biasing Circuit
- **Gate Bias ($V_{GS}$)**: A stable 2.0V - 3.0V is required to set the quiescent current ($I_{DQ}$) to roughly 50-100mA.
- **Implementation**: Use a multi-turn trimmer potentiometer connected to a 5V regulated rail, decoupled with a 10uF tantalum and 100nF ceramic capacitor.

### 2.3 Output Matching & DC Feed
- **DC Feed (RFC)**: A 1.0uH RF Choke (high current rated) supplies 7.2V to the Drain.
- **Output Match**: Pi-Network.
    - $C_1$: 33pF.
    - $L$: 120nH.
    - $C_2$: 27pF.
- **Goal**: Transform the Drain impedance to the 50 Ohm load.

---

## 3. Harmonic Suppression (LPF)

Because 1W is significant power, a **7-pole Low Pass Filter** is required to keep the signal within Amateur Radio spectral limits.
- **Cutoff Frequency ($f_c$)**: 55 MHz.
- **Rejection**: >60dB at 100MHz (2nd harmonic) and 150MHz (3rd harmonic).
- **Topology**: C-L-C-L-C-L-C (Butterworth).

---

## 4. Thermal & PCB Layout Guidelines

> [!IMPORTANT]
> **Thermal Vias**: The SOT-89 package dissipates heat through the **Source tab (Pin 2)**. You MUST place at least 9-16 small thermal vias (0.3mm) directly under this tab, connecting it to a large bottom-side ground plane.

> [!WARNING]
> **Stability**: VHF amplifiers can easily oscillate. Keep all traces as short as possible. Use a solid, unbroken ground plane under the entire PA section.

### Layout Checklist:
1. **Input/Output Isolation**: Keep the input matching components physically separated from the output matching to prevent feedback loops.
2. **Decoupling**: Place the 100nF and 1nF bypass capacitors as close as possible to the RFC and Gate bias pins.
3. **Copper Weight**: Use 1oz or 2oz copper to handle the ~300mA current and improve thermal spread.

---

## 5. Performance Targets
- **Input Power**: +10dBm (10mW).
- **Output Power**: +30dBm (1.0W).
- **Efficiency**: 45% - 55%.
- **Operating Voltage**: 6.0V - 8.4V (2S LiPo compatible).

---
*Reference: Mitsubishi RD01MUS1 Datasheet & VHF RF Design Principles.*
