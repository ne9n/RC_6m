# RC_6m: 6-Meter Ham Band RC Transceiver

A modern, high-performance digital radio control and telemetry system designed for the **50MHz (6-meter)** Amateur Radio band.

## Overview

This project aims to provide a long-range, 2-way digital link for radio control airplanes using the amateur 6m band. Unlike traditional 2.4GHz systems, the 50MHz band offers superior signal propagation and penetration, making it ideal for specialized long-range or high-reliability RC applications.

## Key Features

- **Frequency Band**: 50.0 – 54.0 MHz (Amateur Radio 6m).
- **Architecture**: Discrete RF design using the **Si5351A** frequency synthesizer and **SA612** mixer.
- **Modulation**: GFSK (Gaussian Frequency Shift Keying) for high spectral efficiency.
- **TX Power**: 1 Watt Ground Station (TX) / ~25mW Airplane Telemetry (RX).
- **2-Way Communication**: Integrated control and telemetry link.
- **Modern Logic**: Compatible with **ExpressLRS (ELRS)** packet structures.

## Documentation

For a detailed technical breakdown, including the block diagram, parts list, and RF architecture, see:
- [6m Ham RC Transceiver Design](6m_ham_rc_transceiver_design.md)

## Requirements

> [!IMPORTANT]
> **Amateur Radio License Required**: Operation on the 50MHz band requires a valid Amateur Radio license (e.g., Technician class or higher in the US).

---
*Created by Antigravity AI for ne9n.*
