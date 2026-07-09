#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <U8g2lib.h>

// --- Configuration ---
#define I2C_SDA 8
#define I2C_SCL 9

// --- Hardware Objects ---
Adafruit_ADS1115 ads;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ I2C_SCL, /* data=*/ I2C_SDA);

// --- Data Structures ---
struct StickData {
  int aileron;
  int elevator;
  int throttle;
  int rudder;
  float plane_vbatt = 12.6; // Mock telemetry
  int link_quality = 100;    // Mock telemetry
};

StickData sticks;

// --- Setup ---
void setup() {
  Serial.begin(115200);
  
  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize OLED
  u8g2.begin();
  
  // Initialize ADS1115
  ads.setGain(GAIN_ONE);        // +/- 4.096V range
  ads.setDataRate(RATE_ADS1115_860SPS); // High speed for RC
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  Serial.println("Kraft Digital Encoder Initialized.");
}

// --- Main Loop ---
void loop() {
  // 1. Read Sticks (16-bit)
  sticks.aileron  = ads.readADC_SingleEnded(0);
  sticks.elevator = ads.readADC_SingleEnded(1);
  sticks.throttle = ads.readADC_SingleEnded(2);
  sticks.rudder   = ads.readADC_SingleEnded(3);

  // 2. Map values (Example mapping to 1000-2000us)
  // Note: Calibration logic would go here
  int ail_mapped = map(sticks.aileron, 0, 26000, 1000, 2000);
  int ele_mapped = map(sticks.elevator, 0, 26000, 1000, 2000);

  // 3. Update Display
  u8g2.clearBuffer();
  drawStatusHeader();
  drawStickMonitor(ail_mapped, ele_mapped);
  drawTelemetryFooter();
  u8g2.sendBuffer();

  delay(20); // ~50Hz refresh rate
}

// --- UI Drawing Functions ---

void drawStatusHeader() {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "NE9N-TX");
  u8g2.drawStr(50, 10, "8.4V");
  u8g2.setCursor(90, 10);
  u8g2.print("LQ:");
  u8g2.print(sticks.link_quality);
  u8g2.print("%");
  u8g2.drawHLine(0, 12, 128);
}

void drawStickMonitor(int ail, int ele) {
  // Draw Throttle Bar (Simple Example)
  u8g2.drawStr(0, 30, "THR:");
  u8g2.drawFrame(30, 22, 40, 10);
  int thr_w = map(sticks.throttle, 0, 26000, 0, 38);
  u8g2.drawBox(31, 23, thr_w, 8);

  // Draw Ail/Ele Crosshair
  int cx = 100;
  int cy = 35;
  u8g2.drawFrame(cx-15, cy-15, 30, 30);
  u8g2.drawVLine(cx, cy-15, 30);
  u8g2.drawHLine(cx-15, cy, 30);
  
  // Draw stick position dot
  int dot_x = map(ail, 1000, 2000, cx-14, cx+14);
  int dot_y = map(ele, 1000, 2000, cy+14, cy-14);
  u8g2.drawDisc(dot_x, dot_y, 2);
}

void drawTelemetryFooter() {
  u8g2.drawHLine(0, 52, 128);
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setCursor(0, 62);
  u8g2.print("PLANE: "); u8g2.print(sticks.plane_vbatt); u8g2.print("V");
  u8g2.drawStr(75, 62, "RATE: HIGH");
}
