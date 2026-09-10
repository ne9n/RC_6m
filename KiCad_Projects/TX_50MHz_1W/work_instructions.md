# KiCad Schematic Wiring Instructions

## Status

### 📡 Transmitter — `TX_50MHz_1W.kicad_sch` — ✅ Fully wired (2026-09-09)

The TX schematic was rebuilt end-to-end: every symbol now uses its real `Kraft6M:*` library
definition (with actual pins, not the empty placeholder cache), reference designators are
assigned (U1–U8, Q1, FL1, Y1, C1–C11, L1–L5, J1–J11), and all nets are connected —
the RF chain (Si5351A → gate-match → PA → T/R switch → 7-pole LPF → antenna, and the
LNA → mixer → IF filter → ESP32 telemetry return path), the I2C bus (ESP32 ↔ Si5351A ↔
dual ADS1115 ↔ OLED), the 7 stick/aux connectors, and power/ground.

Four library parts that didn't exist yet were added to `KiCad_Libraries/Kraft6M.kicad_sym`
to make this possible: `C_10nF_0603`, `L_150nH_0603` (PA gate-match components called out in
the blueprint but never modeled), `Crystal_26MHz`, `OLED_SH1106_I2C`, plus generic
`JST_XH_2/3/4Pin` and `SMA_Edge` connector symbols.

**Open it in KiCad and check:**
- Run **ERC** (`Inspect → Electrical Rules Checker`) — this was hand-verified for pin coverage
  and valid syntax outside KiCad, but ERC is the authoritative check.
- Component placement is functional but not laid out for a clean PCB import — feel free to
  reposition for readability; the wires will follow if you drag with the wire attached (`G`).
- **Known simplifications to revisit:** the battery-voltage sense line on ADS1115 #2 (`AIN3`)
  is left as a `VBATT_SENSE` net with no divider resistors yet — Kraft6M library has no
  resistor symbol defined. ESP32 `EN` is tied straight to 3V3 (no pull-up R + cap, same
  library gap). Add a generic `Device:R` resistor to the library (or via KiCad's built-in
  `Device` lib) before laying out the PCB.

### 🛩️ Receiver (Airborne) — `rx_n.kicad_sch` — ⬜ Not yet wired

Still requires the manual pass below.

1. **Move (`G`)** the `BGS12PL6` Switch to the Antenna input.
2. Wire **Antenna** → **Switch ANT (Pin 5)**.
3. Wire **Switch RF2 (Pin 1)** → **LNA Input (SPF5043Z Pin 1)**.
4. Wire **Si5351A CLK1 (Pin 7)** → **Switch RF1 (Pin 3)**.
5. Wire **ESP32 GPIO 10** → **Switch CTRL (Pin 4)**.
6. **Move (`G`)** the `INA219` sensor near the Power input block.
7. Wire **INA219 IN+ (Pin 1)** → **BATT+**.
8. Wire **INA219 IN- (Pin 2)** → **ESC / Main Power Rail**.
9. Wire **INA219 SDA / SCL** to the **ESP32 I2C pins**.

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
