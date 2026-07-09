/*
 * Kraft-6M: Si5351A GFSK Modulation Driver
 * This module handles the "pulling" of frequencies to create a digital link.
 */

#include <si5351.h>
#include <Wire.h>

extern Si5351 si5351;

// Configuration
#define CENTER_FREQ 50100000ULL // 50.1 MHz in centi-Hz
#define DEVIATION   500000ULL   // 5 kHz deviation

/**
 * Sets the transmitter to a specific bit state
 * @param state: true for Logic 1 (+5kHz), false for Logic 0 (-5kHz)
 */
void setRFState(bool state) {
    uint64_t targetFreq = state ? (CENTER_FREQ + DEVIATION) : (CENTER_FREQ - DEVIATION);
    
    // Using frequency update (Note: Si5351 library uses 0.01Hz units)
    si5351.set_freq(targetFreq * 100ULL, SI5351_CLK0);
}

/**
 * Transmits a raw byte via GFSK bit-banging
 * Uses blocking delay for precise bit timing (simplified example)
 */
void transmitByte(uint8_t data, uint32_t bitDurationUs) {
    for (int i = 7; i >= 0; i--) {
        bool bit = (data >> i) & 0x01;
        setRFState(bit);
        delayMicroseconds(bitDurationUs);
    }
}

/**
 * Initialize the RF Modulator for 50MHz
 */
void initModulator() {
    if (!si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0)) {
        Serial.println("Si5351 Init Failed!");
        return;
    }
    
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
    si5351.set_freq(CENTER_FREQ * 100ULL, SI5351_CLK0);
    si5351.output_enable(SI5351_CLK0, 1);
}
