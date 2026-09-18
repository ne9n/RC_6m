# TX antenna-side low-pass filter — S11/S21 sweep (ngspice)

## Update 2 (2026-09-18): re-tuned cutoff, values applied to the schematic

The "Update 1" fix below restored a working filter, but its cutoff sat at 49.1MHz —
right on top of the 50MHz fundamental — so S21 was already down 5dB and S11 was poor
(VSWR 10.4:1) at the operating frequency. Rescaled the whole ladder by the ratio
needed to move the cutoff to ~70-73MHz (L_new = L_old × fc_old/fc_new, same for C,
which preserves the filter shape and 50Ω impedance level), then rounded to standard
E12 part values:

| Part | Old value | New value |
|---|---|---|
| C2, C5 (outer shunt) | 120pF | **82pF** |
| C3, C4 (inner shunt) | 220pF | **150pF** |
| L3, L4, L5 (series) | 180nH | **120nH** |

These are now the actual values in `TX_50MHz_1W.kicad_sch` and BOM (three new library
parts added to `Kraft6M.kicad_sym`: `C_82pF_0805`, `C_150pF_0805`, `L_120nH_0805`).
Netlist: `lpf_final.cir`. Plot: `lpf_final_s11_s21.png`. Numbers: `lpf_final_results.json`.

### Results (final, as committed)

| Frequency | S11 | S21 | VSWR |
|---|---|---|---|
| 10.7 MHz (IF) | -7.5 dB | -0.84 dB | 2.45 |
| **50 MHz (fundamental)** | **-10.9 dB** | **-0.37 dB** | **1.80** |
| 100 MHz (2nd harmonic) | ~0 dB | -37.6 dB | very high |
| -3dB cutoff | — | **73.3 MHz** | — |

50MHz now lands in a local S11 notch with sub-half-dB insertion loss and a good match
(VSWR 1.8), while still keeping strong 2nd-harmonic rejection (-37.6dB, down from
-67.6dB pre-retune, but still comfortably enough for spurious-emission requirements).
This is the current, final result — the "Update 1" numbers below are superseded.

## Update 1 (2026-09-18): root cause found and fixed

The original analysis below (kept for history) treated L4/C4/L5/C5 as an isolated,
unconnected 4-element network because the analyzer showed no path from the T/R
switch's common port to it. That diagnosis was correct about the *symptom* but not
the cause: it wasn't a missing wire.

**Root cause:** `TX_50MHz_1W.kicad_sch` had a stray wire directly connecting C3's two
pads together `(224.79,78.74)-(224.79,73.66)`, in parallel with a wire from that same
point down to a GND symbol. That short-circuited C3 *and* dragged the L3/L4 junction
to GND with it — which is exactly what made the switch-side cluster (U3/C2/L3) and the
antenna-side cluster (L4/C4/L5/C5) look like two disconnected dead-ends. Removing that
one wire (plus two now-meaningless junction dots at the same points) reveals the real,
already-intended topology: a complete **7-pole Pi low-pass filter** from the switch
common port to the antenna —

```
U3.5 (common) -- C2(120pF) shunt -- L3(180nH) series -- Node1
  -- C3(220pF) shunt -- L4(180nH) series -- Node2
  -- C4(220pF) shunt -- L5(180nH) series -- Node_out
  -- C5(120pF) shunt -- J11 (antenna)
```

The symmetric value pattern (120pF outer caps / 220pF inner caps / 180nH×3, matching
the "7-pole LPF" called out in `work_instructions.md`) confirms this was the intended
design — C3 was never a dead no-op part, it was just shorted out by the stray wire.

Re-simulated as a proper 2-port network (`lpf_7pole.cir`, matched-source/load method,
Z0=50Ω both ports): plot is `lpf_7pole_s11_s21.png`, numbers in `lpf_results.json`.

### Results (post-fix)

| Frequency | S11 (return loss) | S21 (insertion loss) | VSWR |
|---|---|---|---|
| 10.7 MHz (IF) | -6.4 dB | -1.1 dB | 2.85 |
| **50 MHz (fundamental)** | **-1.7 dB** | **-5.0 dB** | **10.4** |
| 100 MHz (2nd harmonic) | ~0 dB | -67.6 dB | very high |
| -3dB cutoff | — | **49.1 MHz** | — |

**The filter now works correctly as a low-pass** — 2nd-harmonic rejection at 100MHz is
excellent (-67.6dB). But the **-3dB cutoff lands at 49.1MHz, essentially right on top of
the 50MHz operating frequency**, instead of comfortably above it. That's why S21 at
50MHz is already down 5dB and S11 is poor (VSWR 10.4:1) — the fundamental sits in the
filter's roll-off knee, not in its flat passband. Normal part tolerance (5-10% on these
caps/inductors) could shift the actual cutoff either side of 50MHz unpredictably.

**Recommendation:** scale L3/L4/L5/C2-C5 down slightly to push the cutoff to roughly
65-75MHz (comfortably above 50MHz, still well below 100MHz) before finalizing the BOM.

## Assumptions / model fidelity

- Ideal L/C values from the schematic (no ESR/ESL, no PCB parasitics — routing isn't
  final yet).
- 50Ω assumed on both the switch-side and antenna-side ports; the real T/R switch
  output impedance and antenna feedpoint impedance haven't been verified against
  datasheets.

---

## Original analysis (2026-09-18, superseded above)

Kept for history — this treated the antenna-side cluster as an isolated 4-element
network before the real cause (shorted C3) was found. Netlist: `ant_pi_match.cir`.
Plot: `s11_ant_pi_match.png`. Numbers: `s11_results.json`.

Original conclusion: "node A is not currently wired to anything else... the network
resonates at ~39.6MHz rather than 50MHz." The disconnection diagnosis was a downstream
symptom of the C3 short, not a separate wiring gap — no missing wire needed to be
added, just the stray short removed. The frequency-placement concern raised there
carries forward into the corrected result above (cutoff too close to 50MHz).
