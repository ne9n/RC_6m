# Antenna pi-match network — S11 sweep (ngspice)

Ran on 2026-09-18 against `TX_50MHz_1W.kicad_sch` as a follow-up to the schematic/PCB
review. Netlist: `ant_pi_match.cir`. Plot: `s11_ant_pi_match.png`. Numeric results at
the IF/fundamental/2nd-harmonic frequencies: `s11_results.json`.

## What was simulated

The analyzer's `detect_rf_matching` pass flagged **L4/C4/L5/C5** as a pi-match network
near the antenna connector (J11). Tracing the actual net connectivity in the schematic:

```
node A --[L4 || C4, both shunt to GND]-- L5 (series) --[C5 shunt to GND]-- node B -- J11 (ANT)
```

This was simulated as a standalone 2-port network: a 1A AC current source injected at
node A to compute `Zin(f)`, with node B terminated in an assumed 50Ω antenna load.
`S11 = (Zin - 50) / (Zin + 50)`.

**Important caveat found during this review:** node A is not currently wired to
anything else in the schematic. The T/R switch's common port (U3 pin 5, "RFin") only
connects to L3/C2 (both wired as dead-end shunt elements to GND), and that net has no
connection to node A. So as drawn, this network isn't actually in the RF path yet —
these results describe the network's own response assuming a 50Ω source *would* drive
node A, not the as-built board. Also note C3 (220pF, sits next to L3/C2) has both pads
tied to GND — it does nothing electrically and should be re-checked.

## Results

| Frequency | S11 | VSWR |
|---|---|---|
| 10.7 MHz (IF) | -1.6 dB | 11.0 |
| **50 MHz (fundamental, intended TX frequency)** | **-2.5 dB** | **6.9** |
| 100 MHz (2nd harmonic) | -0.008 dB | 2193 |
| Best match: **39.6 MHz** | -24.1 dB | 1.13 |

The network's actual resonance sits at **~39.6 MHz**, not the board's 50MHz design
frequency — at 50MHz it presents a poor match (VSWR 6.9:1, meaning roughly half the
forward power would reflect if this network were actually in the signal path). The
100MHz point is even worse in absolute terms but VSWR is a red herring there since S11
is near 0dB by coincidence of the impedance angle, not a good match.

## Assumptions / model fidelity

- Ideal L/C values from the schematic (no ESR/ESL, no PCB parasitics — routing isn't
  done yet).
- 50Ω assumed on both the switch-side (node A) and antenna-side (node B) ports. The
  real antenna and the real T/R switch output impedance haven't been verified against
  datasheets.
- This validates the *network's own* frequency response, not the as-wired board (see
  caveat above).

## Follow-ups this points to

1. Wire the switch common port (U3 pin 5) through to this matching network's input
   (node A) — currently missing.
2. Remove or replace C3 (both pads on GND, no-op).
3. Given the network resonates at ~40MHz rather than 50MHz, re-check the L4/C4/L5/C5
   values against the intended 50MHz design target before finalizing the BOM.
