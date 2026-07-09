# Firmware Implementation Guide (50MHz FSK)

This document provides example code and logic for implementing FSK modulation on the Si5351A and demodulation on the STM32/ESP32.

## 1. Si5351A FSK Modulation (Transmitter)

The Si5351A does not have a native "FSK" pin. We achieve modulation by rapidly updating the frequency registers over I2C.

### Example Code (C++/Arduino)
```cpp
#include <si5351.h>
#include <Wire.h>

Si5351 si5351;

// Frequencies for 50.800 MHz RC Window
const uint64_t center_freq = 5080000000ULL; // in 0.01Hz units
const uint32_t fsk_deviation = 240000;      // 2.4kHz deviation

void setup() {
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  si5351.set_freq(center_freq, SI5351_CLK0);
}

// Function to send a single bit (Very simplified)
void sendBit(bool bit) {
  uint64_t target_freq = bit ? (center_freq + fsk_deviation) : (center_freq - fsk_deviation);
  
  // Directly update the Multisynth frequency registers
  // In a real ELRS-like implementation, use a high-speed I2C library
  si5351.set_freq(target_freq, SI5351_CLK0);
}

void loop() {
  // Example bitstream
  sendBit(1); delayMicroseconds(416); // 2400 baud
  sendBit(0); delayMicroseconds(416);
}
```

### Advanced Implementation: GFSK
To implement **Gaussian** FSK (GFSK), instead of jumping directly between `Freq+Shift` and `Freq-Shift`, the MCU should ramp the frequency using a lookup table (SINE or GAUSSIAN) to smooth the transitions. This significantly reduces occupied bandwidth.

---

## 2. Receiver Demodulation (Software Defined Radio)

The 10.7MHz IF signal is fed into the STM32's ADC.

### Algorithm Steps:
1. **Undersampling**: Sample the 10.7MHz signal at a rate that allows for digital down-conversion (e.g., 2.5Msps).
2. **I/Q Conversion**: Multiply the sampled signal by a digital Local Oscillator (LO) to get In-phase (I) and Quadrature (Q) components.
3. **Low Pass Filter**: Remove the high-frequency mixing products.
4. **Phase Discriminator**: Calculate the change in phase between consecutive samples:
   $\Delta\theta = \text{atan2}(I_n Q_{n-1} - Q_n I_{n-1}, I_n I_{n-1} + Q_n Q_{n-1})$
5. **Slicer**: Convert the phase change back into bits (1 or 0).

---

## 3. Packet Structure (ELRS Compatible)

To ensure reliability, use a packet structure similar to ExpressLRS:
- **Preamble**: 4-8 bytes of `0xAA` for clock recovery.
- **Sync Word**: 2-byte unique ID to prevent cross-talk.
- **Payload**: Control channels (AIL, ELE, THR, RUD) + Telemetry.
- **CRC**: 2-byte Cyclic Redundancy Check for error detection.

---
*Note: High-speed I2C (400kHz or 1MHz) is mandatory for the Si5351A to support baud rates above 1200 bps.*
