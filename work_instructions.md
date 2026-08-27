# KiCad Schematic Wiring Instructions

## Why Manual Wiring is Required

KiCad schematics (`.kicad_sch`) are 2D geometric drawings. While Antigravity can generate netlists and inject component symbols into the schematic files, it cannot safely draw the physical wires between them. 

Automated wire routing without spatial awareness of the canvas would result in wires crossing over components, obscuring text, and creating unintended short circuits. Therefore, placing the injected components and drawing the connecting lines is a step that requires a human in the KiCad GUI.

---

## Wiring Checklist

The telemetry components have been injected into your schematic sheets. Open KiCad, locate the un-wired components near the top of the sheets, and use the **Wire Tool (`W`)** and the **Move Tool (`G` to drag with wires, `M` to move freely)** to make the following connections.

### 📡 Transmitter (Ground Station) - `TX_50MHz_1W.kicad_sch`

1. **Move (`G`)** the new `BGS12PL6` Switch between the `RD01MUS2` PA output and the Antenna.
2. Wire **PA Drain** → **Switch RF1 (Pin 3)**.
3. Wire **Switch ANT (Pin 5)** → **Low Pass Filter**.
4. Wire **Switch CTRL (Pin 4)** → **ESP32 GPIO 10**.
5. Wire **Switch RF2 (Pin 1)** → **SPF5043Z LNA Input (Pin 1)**.
6. Wire **LNA Output (Pin 3)** → **LT5560 Mixer IN+ (Pin 1)**.
7. Wire **Si5351A CLK1 (Pin 7)** → **LT5560 Mixer LO+ (Pin 7)**.
8. Wire **LT5560 IF Output (Pin 5)** → **10.7MHz Filter Input**.
9. Wire **10.7MHz Filter Output** → **ESP32 ADC Pin**.

### 🛩️ Receiver (Airborne) - `rx_n.kicad_sch`

1. **Move (`G`)** the `BGS12PL6` Switch to the Antenna input.
2. Wire **Antenna** → **Switch ANT (Pin 5)**.
3. Wire **Switch RF2 (Pin 1)** → **LNA Input (SPF5043Z Pin 1)**.
4. Wire **Si5351A CLK1 (Pin 7)** → **Switch RF1 (Pin 3)**.
5. Wire **ESP32 GPIO 10** → **Switch CTRL (Pin 4)**.
6. **Move (`G`)** the `INA219` sensor near the Power input block.
7. Wire **INA219 IN+ (Pin 1)** → **BATT+**.
8. Wire **INA219 IN- (Pin 2)** → **ESC / Main Power Rail**.
9. Wire **INA219 SDA / SCL** to the **ESP32 I2C pins**.
