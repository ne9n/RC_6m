# KiCad Schematic Wiring Instructions

## Status

### 📻 Telemetry Receiver — `TX_50MHz_1W.kicad_sch` — ✅ Si4463 chain in schematic, PCB not yet updated (2026-09-28)

The telemetry receive path is now *LT5560 upconverter → ~169MHz LC IF band-pass → Si4463 FSK
receiver → SPI* (was *LT5560 → 10.7MHz FL1 → ESP32 ADC*). Si5351A CLK1 is a fixed
**118.2MHz LO on PLLB**; CLK0 stays the FSK carrier on PLLA. Rationale, frequency plan and pin
tables: `TX_Hardware/TX_Schematic_Blueprint.md` and `RX_Hardware/RX_Design_Details.md`.

Done in the schematic (direct file edit, verified with `kicad-cli` netlist + ERC, no new errors):
1. FL1 and the `ADC_IN` net deleted; ESP32 **IO10** now has a no-connect flag.
2. LT5560 (U5): IN-/LO-/OUT- were tied straight to GND → now C27/C28 (100pF) and C29
   (10nF) AC grounds. OUT+/OUT- get DC feeds L8/L9 (value TBD), OUT+ → C30 → IF band-pass.
3. SPF5043Z (U4) had no bias feed → L7 1µH RFC from +3V3 (C25 10nF decoupling), and C26
   100pF DC block into the mixer.
4. 3-pole LC IF band-pass L10–L12 / C31–C35 (values TBD), placeholder AN643 match C36/L13/C37.
5. **U13 Si4463-C2A-GM** with Y2 30MHz (3225, 4-pad), C38–C42 decoupling, R13 10k nIRQ pull-up.
6. SPI + control: **SCLK = IO12, MOSI = IO13, MISO = IO14, nSEL = IO15, nIRQ = IO16, SDN = IO17**.
7. **ESP32 symbol pad numbers corrected**: IO8/IO9/IO10/IO11 were on pads 11/12/13/14 (really
   IO18, IO8, USB D-, USB D+) → now 12/17/18/19, in the schematic and in `Kraft6M.kicad_sym`.

**Still to do:**
- *Update PCB from Schematic* (new parts + changed ESP32 pad nets), then place/route them.
- Pick values: IF band-pass (SPICE), LT5560 169MHz output feeds, AN643 match topology/values.
- Unrelated issues seen in the netlist: ADS1115 U7 has SDA/SCL swapped; Q3 base and R9 are
  unconnected; AP2112K (U10) EN is unconnected.
- **If KiCad has this project open, close it without saving before reopening** — the change
  was a direct file edit.

### 🔌 USB Interface — `USB_Interface.kicad_sch` — ✅ Split out + battery charger added (2026-09-21)

The USB-C connector (J12), CP2102N USB-UART bridge (U11), the DTR/RTS auto-reset
transistors (Q2/Q3, R5–R10), and their decoupling caps (C18–C22) were moved out of the
top-level TX schematic into their own hierarchical sub-sheet, referenced from the root
sheet via a `sheet` symbol (Sheetname "USB Interface"). Every net crossing the sheet
boundary (`USB_DP`, `USB_DM`, `VBUS_5V`, `CC1_NET`, `CC2_NET`, `DTR_SIGNAL`, `RTS_SIGNAL`,
`EN`, `BOOT_IO0`, `U0TXD`/`U0RXD`, plus `+3V3`/`GND`) was already a global label or power
symbol, so no sheet pins were needed — the sub-sheet has none.

Added to the sub-sheet: a **USB-C battery charger** (U12, `MCP73831-2-OT`) that takes
`VBUS_5V` and charges the same `+BATT` net the main pack (J8, XT30) is on, so plugging in
USB-C tops up whatever battery is connected there. R11 (10k) sets the PROG current to a
conservative ~100 mA (`Ireg = 1000/R11[kΩ]`) — swap for ~2k (~500 mA) if faster charging of
a larger pack is wanted. D1 is a charge-status LED (lit while charging, STAT pulled low).
C23/C24 are the datasheet-recommended 1 µF bypass caps on VDD/VBAT. A `PWR_FLAG` on
`VBUS_5V` tells ERC that net is driven from outside the schematic (matches the project's
existing convention elsewhere).

**Still to do:** lay out U12/R11/R12/C23/C24/D1 on the PCB (they only exist in the
schematic/netlist so far) and re-run ERC/DRC in KiCad. **If KiCad has this project open,
close it without saving before reopening** — the split was done by direct file edit, and
saving from the old in-memory (still-flat) schematic would overwrite this change.

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
