# 50MHz 1W RC Link: RF Range & Link Budget Analysis

This document provides a theoretical analysis of the expected range and reliability for the 1W 50MHz RC system.

## 1. System Parameters

| Parameter | Value | Note |
| :--- | :--- | :--- |
| **Transmit Power (Pt)** | +30 dBm (1 Watt) | Output after LPF |
| **Transmit Antenna Gain (Gt)** | 2.15 dBi | Standard half-wave whip |
| **Receive Antenna Gain (Gr)** | -3.0 dBi | Shortened wire antenna (compromised) |
| **Receiver Sensitivity (Pr)** | -105 dBm | Typical for LT5560 + SPF5043Z |
| **Operating Frequency** | 50.1 MHz | 6-meter Ham Band |

---

## 2. Link Budget Calculation

### 2.1 Free Space Path Loss (FSPL)
At 50MHz, the wavelength ($\lambda$) is approximately **6.0 meters**.

**Formula**: $FSPL (dB) = 20 \log_{10}(d) + 20 \log_{10}(f) - 147.55$
*(Where d is in meters, f is in Hz)*

### 2.2 Range Estimates (Visual Line of Sight)

| Distance | Path Loss (50MHz) | Fade Margin | Link Status |
| :--- | :--- | :--- | :--- |
| **500 m** | 60.4 dB | 76.7 dB | Solid |
| **1.0 km**| 66.4 dB | 70.7 dB | Solid |
| **5.0 km**| 80.4 dB | 56.7 dB | Reliable |
| **10.0 km**| 86.4 dB | 50.7 dB | Very High Reliability |

**Theoretically, 1W at 50MHz is capable of 20km+ range** if both antennas have clear line-of-sight. For RC model use, the range will be limited by the pilot's vision long before the signal fails.

---

## 3. Advantages of 50MHz over 2.4GHz
1.  **Fresnel Zone Clearance**: 50MHz has a much larger first Fresnel zone, meaning it is less affected by ground reflections at low altitudes.
2.  **Obstacle Penetration**: 6-meter signals wrap around hills, buildings, and trees much better than 2.4GHz microwave signals.
3.  **Ground Effect**: 2.4GHz signals are heavily absorbed by wet grass and soil; 50MHz is far more resilient for low-altitude flying.

---

## 4. Potential Risks & Interference
- **Man-Made Noise**: 50MHz can be susceptible to noise from electric motors and high-power switching supplies.
- **Atmospheric Conditions**: During periods of high solar activity (Sporadic-E), 50MHz can "skip" over the horizon, potentially causing interference from stations hundreds of miles away.
- **Harmonics**: 1W is significant power. The **7-pole Low Pass Filter** is mandatory to prevent interference with television and public safety bands.

---

## 5. Conclusion
With a **+50dB fade margin** at 10km, this system will be virtually "un-glitchable" at any distance a human pilot can see a model aircraft. The primary limiting factor will be local interference from the airplane's own ESC and servos.

---
*Reference: Friis Transmission Equation & ITU-R P.525-4.*
