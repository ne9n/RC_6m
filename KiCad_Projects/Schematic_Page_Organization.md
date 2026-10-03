# KiCad Schematic Page Organization

**Status:** TX and RX hierarchies are implemented and netlist-verified. TX now has a dedicated Battery + Power page; RX has Controller/I/O, RF Front End, and IF Radio child pages.

## Design Rules

- Keep the transmitter and airborne receiver as separate KiCad projects.
- Use one root sheet per project as a block-level overview. Put circuitry on child sheets by function and signal flow.
- Use hierarchical labels/sheet pins or global labels for signals that cross child-sheet boundaries. Keep nets local when both endpoints are on the same sheet.
- Use global power symbols for power rails and ground rather than adding a hierarchy pin for every power connection.
- Preserve reference designators where practical. Run ERC and regenerate netlists after moving circuitry.

## Transmitter Project: `TX_50MHz_1W`

| Sheet | File | Contents |
|---|---|---|
| Root | `TX_50MHz_1W.kicad_sch` | TX system overview and named inter-sheet connections to four child sheets. |
| Battery + Power | `TX_Battery_Power.kicad_sch` | J8/J9 battery and 5V inputs, LMR33630 buck converter, AP2112 3.3V regulator, and support passives. |
| Controller + TX RF | `TX_Controller_RF.kicad_sch` | ESP32-S3, ADS1115s, controls/OLED, Si5351A, PA, T/R switch, transmit filter/antenna, and telemetry LNA/mixer. |
| Telemetry IF | `TX_Telemetry_IF.kicad_sch` | 169 MHz IF network/match, Si4463, 30 MHz crystal, and associated passive network. |
| USB Interface | `USB_Interface.kicad_sch` | Existing USB interface and battery charger child sheet. |

### TX Inter-sheet Nets

| Net | From | To | Purpose |
|---|---|---|---|
| `LNA_OUT`, `MIX_IN`, `MIX_IN_N`, `MIX_LO_N`, `MIX_OUT_N`, `IF_OUT_P` | Controller + TX RF / LNA and mixer | Telemetry IF / passive IF network | Analog front-end to IF boundary. |
| `SI_SCLK`, `SI_MOSI`, `SI_MISO`, `SI_nSEL`, `SI_nIRQ`, `SI_SDN` | Controller + TX RF / ESP32-S3 | Telemetry IF / Si4463 | SPI data, chip select, interrupt, and shutdown. |
| `5V`, `V_BATT` | Battery + Power / input and regulators | Controller + TX RF / 5V rail and PA supply | Named root ports connect the battery/power sheet to the controller/RF page. |
| `+3V3`, `GND` | Global power symbols | Controller + TX RF, Telemetry IF, USB Interface, and Battery + Power | Global supply and ground. |
| `EN`, `BOOT_IO0`, `U0RXD`, `U0TXD` | Controller + TX RF / ESP32-S3 | USB Interface / CP2102N and auto-reset | USB serial and reset-control nets. |

I2C and `TR_SW_CTRL` remain local to Controller + TX RF. Cross-sheet signals appear as root sheet pins with matching child hierarchical labels; power rails use global power symbols.

## Receiver Project: `RX_50MHz_SDR`

| Sheet | File | Contents |
|---|---|---|
| Root | `RX_50MHz_SDR.kicad_sch` | System-level overview and child-sheet connections. |
| Controller / I/O | `RX_Controller_IO.kicad_sch` | ESP32-S3, servo headers, INA219, 3.3 V regulator, and MCU-side control. |
| RF Front End | `RX_RF_Front_End.kicad_sch` | Antenna, T/R switch, LNA/matching, 50 MHz filter, mixer, and Si5351A LO / telemetry drive. |
| IF Radio | `RX_IF_Radio.kicad_sch` | 169 MHz IF filter/match and receive-only Si4463. |

### RX Inter-sheet Nets

| Net | From | To | Purpose |
|---|---|---|---|
| `SDA`, `SCL` | Controller / ESP32 and INA219 | RF Front End / Si5351A | I2C control bus. |
| `TR_SW`, `RX_EN` | Controller / ESP32 | RF Front End / switch and mixer | RX/TX path control and mixer enable. |
| `IF_OUT_P`, `MIX_OUT_N` | RF Front End / LT5560 | IF Radio / IF filter | Mixer IF outputs into the 169 MHz filter. |
| `SI_SCLK`, `SI_MOSI`, `SI_MISO`, `SI_nSEL`, `SI_nIRQ`, `SI_SDN` | Controller / ESP32 | IF Radio / Si4463 | SPI, interrupt, and shutdown signals. |

The RX root page now shows named sheet pins and short net-label wires for every cross-sheet signal. Matching hierarchical labels on the child pages connect those ports. RF-only nets, servo signals, USB, and `5V_RAIL` stay local; `3V3` and `GND` use global power symbols.

## Power Nets

Use global power symbols for rails instead of hierarchy pins. The TX schematic uses `GND`, `+3V3`, and `V_BATT`; `5V` is an explicit inter-sheet signal. The RX schematic uses its existing `GND`, `+3V3`/`3V3`, and `5V_RAIL` nets. Keep these names as shown in each schematic.

## Conversion Notes

1. The TX root now exposes the cross-sheet nets as sheet pins; matching hierarchical labels are on each connected child page.
2. The TX and RX tables record root sheet pins and matching child hierarchical labels. Keep one consistent name per signal within each project.
3. The checked-in exported `.net` files are stale: they still show the old 10.7 MHz filter / ADC receive path and do not include the current Si4463 receive chain. Regenerate them from the updated schematics after the hierarchy conversion.
4. Run ERC on each project and inspect the generated netlists to confirm every listed cross-sheet signal reaches the intended pins.

## Related Design References

- [TX schematic blueprint](../TX_Hardware/TX_Schematic_Blueprint.md)
- [RX schematic blueprint](../RX_Hardware/RX_Schematic_Blueprint.md)
- [Project wiring instructions](work_instructions.md)
