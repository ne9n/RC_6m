# Murata CERAFIL® SFE10.7 Series Reference

This document provides the technical specifications for the 10.7 MHz ceramic IF filter used in the RX design.

## 1. Primary Specifications (SFE10.7MS2)

| Parameter | Value |
| :--- | :--- |
| **Center Frequency (fc)** | 10.700 MHz ± 30 kHz |
| **3dB Bandwidth** | 230 kHz ± 50 kHz |
| **20dB Bandwidth** | 600 kHz (max) |
| **Insertion Loss** | 6.0 dB (max) |
| **Input/Output Impedance**| 330 $\Omega$ |
| **Spurious Response** | 40 dB (min) [9-12 MHz] |

## 2. Physical Dimensions & Pinout
**Package Type**: Through-Hole Ceramic (Standard 3-pin)

```text
      _______
     |       |
     | 10.7S |  <-- Marking (Example)
     |_______|
       | | |
       1 2 3
```

| Pin | Function | Connection |
| :--- | :--- | :--- |
| **1** | **Input** | From Mixer OUT+ (LT5560 Pin 5) |
| **2** | **Ground** | System Ground Plane |
| **3** | **Output** | To ESP32 ADC Input |

## 3. Matching Requirements
To achieve the specified bandwidth and ripple, the filter must be terminated with **330 $\Omega$**. 
- In our design, the LT5560 output impedance should be checked. 
- If the MCU ADC input impedance is high, a parallel 330 $\Omega$ resistor to ground may be required at Pin 3 to ensure proper filter performance.

## 4. Official Documentation
Full catalog data can be found in **Murata Catalog No. P61E-6**.
- [Murata Ceramic Filters for FM Receivers](https://www.murata.com/en-global/products/filter/ceramicfilter/catalog)

---
*Reference for the Kraft-6M 50MHz Receiver Project.*
