# KiCad Schematic Wiring Instructions

## Status (2026-09-28): receive chain moved to Si4463 -- schematics updated, PCBs not yet

The receive chain on **both boards** (RX receiver and the TX's telemetry receiver) is now:
SPF5043Z LNA -> LT5560 **upconverter** (Si5351A LO fixed at 118.2MHz) -> ~169MHz LC
band-pass -> AN643 match -> **Si4463-C2A-GM** -> SPI -> ESP32-S3. This replaces the 10.7MHz
ceramic filter and the CD74HC4046A PLL discriminator / ADC path. See
`RX_Hardware/RX_Design_Details.md` and `RX_Hardware/RX_Schematic_Blueprint.md`.

**Both schematics now carry the new chain** (edited 2026-09-28, verified with `kicad-cli`
netlist export + ERC; no new ERC errors). The TX steps 8-9 below (10.7MHz filter -> ESP32 ADC)
are superseded. **Still to do:**
1. **PCBs**: open each project and run *Tools -> Update PCB from Schematic* -- the new parts
   (Si4463, crystal, IF filter, match, decoupling) aren't on either board yet, and pad
   assignments changed (see below).
2. **Component values**: IF band-pass (L/C "TBD"), LT5560 169MHz output feeds, and the Si4463
   RX match are placeholders. Design the band-pass with SPICE; take the match topology and
   values from Silicon Labs AN643 (169MHz) -- the schematic's 3-part match is a placeholder.
3. If KiCad had either project open during the edit, close it **without saving** before reopening.

`RX_50MHz_SDR.kicad_sch` -- fixed while adding the Si4463 (all were real netlist errors):
- Servo pins: the five ESP32 servo pins were shorted onto one net. Now CH1-8 = IO4, IO5,
  IO6, IO7, IO15, IO16, IO1, IO2 -> J5-J12, one net each.
- Si5351A/INA219 I2C now reaches the ESP32 (SDA = IO8, SCL = IO9).
- RF chain: J1 went straight to the LNA past the T/R switch, the LNA never reached the mixer
  (C3 open, mixer IN+ was on the T/R control line), and Si5351 CLK0 drove the mixer's OUT-
  pin. Now J1 -> U6 ANT, U6 RF1 -> C2 -> LNA, C3 -> LT5560 IN+, CLK0 -> LO+, CLK1 -> C1 -> U6
  RF2, C5 -> IN-, C7 -> OUT-.
- T/R switch CTRL = IO10, LT5560 EN (`RX_EN`) = IO21. The custom ESP32 symbol gained IO1,
  IO2, IO13, IO14, IO21.
- Si4463 on SPI: SCLK IO12, MOSI IO11, MISO IO13, nSEL IO14, nIRQ IO17, SDN IO18.

Still wrong in the RX schematic (not touched -- outside this change):
- ESP32 auto-reset: `EN`/`BOOT_IO0` labels dangle; Q1/Q2 aren't connected to anything.
- The CP2102N is on a separate `+3V3` net from the main `3V3` rail (it has no supply).
- Y1 (26MHz) is a 2-pin symbol on the 4-pad 3225 footprint, so Si5351 XB lands on a GND pad.
- C4 (10nF) shunts the LNA output node to GND; it belongs on the 3V3 side of RFC L3.
- No 50MHz band-pass between LNA and mixer, although the block diagram shows one.
- `XTAL_B`/`VIN_LDO` labels dangle; AP2112K VIN has no power flag.

`TX_50MHz_1W.kicad_sch` -- fixed while adding the Si4463:
- **ESP32 pad numbers**: the `Kraft6M:ESP32-S3-WROOM-1` symbol had IO8/IO9/IO10/IO11 on pads
  11/12/13/14 (really IO18, IO8, USB D-, USB D+). Now 12/17/18/19, in both the schematic and
  `KiCad_Libraries/Kraft6M.kicad_sym`. IO12-IO17 added.
- LT5560 IN-/LO-/OUT- were tied straight to GND -- now AC-grounded (C27/C28/C29, as on the RX).
- SPF5043Z LNA had no bias feed and was DC-coupled into the mixer -- added L7 (1uH RFC), C25,
  C26 (DC block).
- FL1 and `ADC_IN` removed; IO10 is now free (no-connect flag).

Still wrong in the TX schematic (not touched): ADS1115 U7 has SDA/SCL swapped relative to the
bus; Q3 base and R9 are unconnected (auto-reset); AP2112K EN unconnected.

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
8. ~~Wire **LT5560 IF Output (Pin 5)** → **10.7MHz Filter Input**.~~ Superseded: → **~169MHz LC BPF** → **Si4463** (see status above).
9. ~~Wire **10.7MHz Filter Output** → **ESP32 ADC Pin**.~~ Superseded: Si4463 connects to the ESP32 over SPI + nIRQ + SDN.

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
