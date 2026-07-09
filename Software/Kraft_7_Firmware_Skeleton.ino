/*
 * Kraft 7 "Restomod" - 50MHz Digital Encoder Firmware (Skeleton)
 * Targets: ESP32-S3 + Dual ADS1115 + Si5351A + SH1106 OLED
 */

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <si5351.h>
#include <U8g2lib.h>

// --- Hardware Addresses ---
#define ADS_MAIN_ADDR 0x48
#define ADS_AUX_ADDR  0x49
#define OLED_ADDR     0x3C

// --- Pins ---
#define I2C_SDA 8
#define I2C_SCL 9

// --- Objects ---
Adafruit_ADS1115 adsMain;
Adafruit_ADS1115 adsAux;
Si5351 si5351;
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

// --- Data Structures ---
struct RC_Frame {
    uint16_t channels[8];
    uint8_t  flags;
    uint8_t  crc;
};

RC_Frame currentFrame;

// --- Functions ---

void setup() {
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);

    // Initialize OLED
    u8x8.begin();
    u8x8.setFont(u8x8_font_artossans8_r);
    u8x8.drawString(0, 0, "KRAFT 7 DIGITAL");

    // Initialize ADCs
    if (!adsMain.begin(ADS_MAIN_ADDR)) Serial.println("Main ADC Fail");
    if (!adsAux.begin(ADS_AUX_ADDR))   Serial.println("Aux ADC Fail");

    // Initialize Si5351
    if (!si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0)) Serial.println("Si5351 Fail");
    si5351.set_freq(5010000000ULL, SI5351_CLK0); // Set to 50.1 MHz
}

void readSticks() {
    // Read Main Sticks (16-bit)
    currentFrame.channels[0] = adsMain.readADC_SingleEnded(0); // Aileron
    currentFrame.channels[1] = adsMain.readADC_SingleEnded(1); // Elevator
    currentFrame.channels[2] = adsMain.readADC_SingleEnded(2); // Throttle
    currentFrame.channels[3] = adsMain.readADC_SingleEnded(3); // Rudder

    // Read Aux Channels
    currentFrame.channels[4] = adsAux.readADC_SingleEnded(0);
    currentFrame.channels[5] = adsAux.readADC_SingleEnded(1);
    currentFrame.channels[6] = adsAux.readADC_SingleEnded(2);
}

void transmitFrame() {
    // 1. Calculate CRC
    // 2. Perform GFSK Modulation via Si5351 Freq Hopping or Pulling
    // 3. Toggle PA Enable pin if used
}

void updateOLED() {
    // Display Battery, RSSI, and Channel Data
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 200) {
        u8x8.setCursor(0, 2);
        u8x8.printf("A:%04d E:%04d", currentFrame.channels[0], currentFrame.channels[1]);
        lastUpdate = millis();
    }
}

void loop() {
    readSticks();
    transmitFrame();
    updateOLED();
    
    // Maintain 50Hz Frame Rate (20ms)
    delay(20); 
}
