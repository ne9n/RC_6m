# Firmware Implementation Guide (50MHz FSK)

This document provides example code and logic for implementing FSK modulation on the Si5351A and packet reception on the ESP32-S3 via an **Si4463** FSK receiver (both the RX board and the TX board's telemetry receiver).

## 1. Si5351A FSK Modulation (Transmitter)

The Si5351A does not have a native "FSK" pin. We achieve modulation by rapidly updating the frequency registers over I2C.

### Example Code (C++/Arduino)
```cpp
#include <si5351.h>
#include <Wire.h>

Si5351 si5351;

// Frequencies for 50.800 MHz RC Window
const uint64_t center_freq = 5080000000ULL; // in 0.01Hz units
const uint32_t fsk_deviation = 500000;      // 5kHz deviation (+-5kHz, project standard;
                                            // older docs said 2.4kHz -- the Si4463 receiver
                                            // is configured for +-5kHz, keep them matched)

void setup() {
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  // CLK0 (FSK carrier) on PLLA, CLK1 (118.2MHz receive LO) on PLLB -- separate PLLs so
  // the per-bit FSK updates to CLK0 never disturb the LO.
  si5351.set_ms_source(SI5351_CLK0, SI5351_PLLA);
  si5351.set_ms_source(SI5351_CLK1, SI5351_PLLB);
  si5351.set_freq(center_freq, SI5351_CLK0);
  si5351.set_freq(11820000000ULL, SI5351_CLK1); // 118.200MHz LO, fixed
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

## 2. Receiver Demodulation

**Current architecture (use this): Si4463 packet receiver over SPI.**
The 6m signal is upconverted by the LT5560 mixer (Si5351A LO fixed at 118.200MHz,
IF = RF + LO, so 50.1-51.0MHz lands at 168.3-169.2MHz with no spectral inversion) and
demodulated by an **Si4463-C2A-GM** in hardware. The ESP32 never sees analog IF or baseband
-- it reads finished packets over SPI (<=10MHz, mode 0). Same code on the RX board and on the
TX board's telemetry receiver. Receive flow:
1. **Power-up**: drive SDN low, wait for POR, send the WDS / Simplicity Studio Radio
   Configurator output (`radio_config_si4463.h`: POWER_UP + properties) over SPI, polling CTS
   between commands. AFC enabled (worst-case frequency error ~6.8kHz exceeds the +-5kHz
   deviation).
2. **START_RX** on channel N (current hop), fixed packet length (22-byte payload after sync).
3. **nIRQ on PACKET_RX** -> read 22 bytes from the RX FIFO, read the latched RSSI
   (FRR or GET_MODEM_STATUS) -> check CRC8 (poly 0x07) **in firmware** (Si4463 hardware CRC
   disabled) -> update servos (RX board) / telemetry display (TX board).
4. **Hop timer**: START_RX with the next channel number. The LO stays fixed -- no Si5351
   retune or I2C traffic on the receive side per hop.
5. **Failsafe** unchanged: 500ms without a valid packet -> neutral/preset.
6. **RSSI** for the telemetry RSSI / link-quality fields now comes from the Si4463.

Suggested files: `si4463_radio.h/.cpp` (SPI driver: CTS poll, command/response, FIFO read,
START_RX) and `radio_config_si4463.h` (WDS output). Pin map and hardware details:
`RX_Hardware/RX_Design_Details.md` and `RX_Hardware/RX_Schematic_Blueprint.md`.

**Superseded (kept for reference, do not use): CD74HC4046A PLL discriminator.**
`fsk_baseband_slicer.h`, `RX_FSK_Demod_PLL.ino` and `test/fsk_baseband_slicer_selftest.cpp`
implement a baseband bit slicer behind a 4046 PLL demodulating the 10.7MHz IF. Dropped
because the HC4046 VCO is marginal/out of spec at 10.7MHz on 3.3V, and it is demod-only (no
AFC, RSSI, sync detect or packet handling), leaving all of that to firmware.

**Superseded (kept for reference, do not use): IF-undersampling.**
`fsk_demod_core.h`, `RX_FSK_Demod.ino` and `test/fsk_demod_selftest.cpp` sample the raw
10.7MHz IF with the ADC and demodulate in software. The algorithm is verified, but the
ESP32-S3 ADC is capped at 83,333 Hz (`SOC_ADC_SAMPLE_FREQ_THRES_HIGH`), far too slow for the
~230kHz-wide 10.7MHz ceramic filter output.

---

## 3. Packet Structure (ELRS Compatible)

To ensure reliability, use a packet structure similar to ExpressLRS:
- **Preamble**: at least 5 bytes (40 bits) of alternating `0x55`/`0xAA` so the Si4463's AFC and preamble detector settle.
- **Sync Word**: `0x1D4A` (2 bytes), detected by the Si4463 packet handler.
- **Payload**: Control channels (AIL, ELE, THR, RUD) + Telemetry, fixed length.
- **CRC**: CRC8 (poly 0x07) computed and checked in firmware (see `Firmware_Protocol_Spec.md`); Si4463 hardware CRC disabled.
- **Encoding**: NRZ, no whitening. The Si4463 can't decode 8b/10b; hardware Manchester is the fallback if long runs cause bit errors.

---
*Note: High-speed I2C (400kHz or 1MHz) is mandatory for the Si5351A to support baud rates above 1200 bps.*
