# PCB Layout Strategy (50MHz RF Design)

Designing a 50MHz transceiver requires careful attention to grounding, isolation, and thermal management to achieve the 1W output target while maintaining receiver sensitivity.

## 1. Stackup Recommendation
- **4-Layer PCB** is highly recommended.
  - **Layer 1 (Top)**: Signal, RF traces, and components.
  - **Layer 2**: Solid Ground Plane (Critical for RF return paths).
  - **Layer 3**: Power Rails (7.2V PA, 3.3V Digital).
  - **Layer 4 (Bottom)**: Solid Ground Plane and non-critical signals.

---

## 2. RF Grounding & Isolation
- **Solid Ground Plane**: Do not split the ground plane. Use a single, continuous plane on Layer 2 to provide the lowest possible impedance for RF return currents.
- **Vias**: Use "Via Stitching" around the edge of the board and between RF sections to connect the Top and Bottom ground pours.
- **Isolation**: Keep the **TX PA section** physically separated from the **RX LNA section**. The T/R switch should be the only point where they meet.

---

## 3. Power Amplifier (PA) Layout
- **Thermal Vias**: The RD01MUS2 dissipates heat through the Source tab. Place a 4x4 grid of 0.3mm thermal vias directly under the tab.
- **Output Trace**: The trace from the PA to the LPF and Antenna should be **50 Ohm Impedance Controlled**. On a standard 1.6mm FR4 4-layer board, this is typically ~0.3mm to 0.5mm wide (verify with your fab's stackup).
- **Decoupling**: Place 10uF and 100nF capacitors as close as possible to the Drain DC feed (RFC).

---

## 4. Receiver (RX) Layout
- **LNA Proximity**: The SPF5043Z LNA should be placed as close as possible to the RF Switch/Antenna connector.
- **Shielding**: If possible, leave space for a small SMD "Shield Can" over the LNA and Mixer stage to prevent pickup of noise from the MCU or ESCs.

---

## 5. Digital Noise Reduction
- **Clock Generators**: Keep the Si5351A and its 26MHz crystal far away from the LNA input.
- **I2C Routing**: Route the I2C lines (SDA/SCL) away from the RF path. Use 10k pull-ups and consider adding small 47pF capacitors if digital noise is visible in the RX floor.

---

## 6. Antenna Connector
- **Type**: Use an **SMA** or **BNC** connector for the Ground Station. Use a simple through-hole solder pad for the 1.5m wire on the Airplane RX.
- **Grounding**: Ensure the connector ground tabs are soldered to both the top and bottom planes with multiple vias.

---
*Reference: High-Speed Digital & RF Design Principles (Henry Ott / Howard Johnson).*
