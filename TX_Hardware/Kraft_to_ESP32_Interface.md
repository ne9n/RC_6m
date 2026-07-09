# Kraft 7 Transmitter to ESP32 Interface

This guide describes how to connect a vintage Kraft 7-channel transmitter to a modern ESP32-based 50MHz transceiver deck.

## 1. Interface Requirements
- **Kraft Signal**: Analog PPM (Pulse Position Modulation).
- **ESP32 Input**: GPIO 4 (Interrupt-enabled).
- **Logic Level**: 3.3V (Requires level shifting from Kraft's 5V-9V signal).

## 2. Hardware Connections

### 2.1 Level Shifting (Kraft to ESP32)
Since the Kraft encoder may output signals up to 9.6V, a simple voltage divider is required to protect the ESP32:
- **R1 (Series)**: 22k Ohm.
- **R2 (Shunt to GND)**: 10k Ohm.
- **Output**: This provides ~3.0V from a 9.6V source, safe for the ESP32.

### 2.2 Power Supply
- Use a small **Step-down (Buck) Converter** to drop the Kraft's 9.6V-12V battery voltage to **5V** for the ESP32.
- The ESP32's on-board regulator will provide the **3.3V** needed for the Si5351A.

## 3. Firmware Logic (PPM Decoding)

The ESP32 must accurately measure the pulse widths from the Kraft encoder.

### Example PPM Decoder Snippet
```cpp
volatile uint32_t channel_values[8];
volatile uint32_t last_time = 0;
volatile uint8_t current_channel = 0;

void IRAM_ATTR ppm_interrupt() {
    uint32_t now = micros();
    uint32_t duration = now - last_time;
    last_time = now;

    if (duration > 3000) { // Sync Pulse detected
        current_channel = 0;
    } else {
        if (current_channel < 8) {
            channel_values[current_channel] = duration;
            current_channel++;
        }
    }
}

void setup() {
    pinMode(4, INPUT_PULLDOWN);
    attachInterrupt(4, ppm_interrupt, RISING);
}
```

## 4. Signal Integration
1. **Decode**: ESP32 reads 7 channels from the Kraft sticks.
2. **Packetize**: Encapsulate the 7 channels into a digital packet with CRC.
3. **Transmit**: Send the packet via the 50MHz Si5351A RF stage.

---
*Note: Ensure the Kraft internal RF section is disconnected or the crystal is removed to prevent interference and save battery power.*
