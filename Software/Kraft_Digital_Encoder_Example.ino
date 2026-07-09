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
  
  // Digital Trim Offsets
  int trim_ail = 0;
  int trim_ele = 0;
  int trim_rud = 0;

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

  // Setup Trim Pins (Internal Pullups)
  pinMode(10, INPUT_PULLUP); // Trim Ail Left
  pinMode(11, INPUT_PULLUP); // Trim Ail Right

  Serial.println("Kraft Digital Encoder Initialized.");
}

// --- Main Loop ---
void loop() {
  // 1. Read Sticks (16-bit)
  int raw_ail = ads.readADC_SingleEnded(0);
  int raw_ele = ads.readADC_SingleEnded(1);
  int raw_thr = ads.readADC_SingleEnded(2);
  int raw_rud = ads.readADC_SingleEnded(3);

  // 2. Handle Digital Trims (Example for Aileron)
  if (digitalRead(10) == LOW) { sticks.trim_ail--; delay(100); }
  if (digitalRead(11) == LOW) { sticks.trim_ail++; delay(100); }

  // 3. Apply Trims & Map values (Example mapping to 1000-2000us)
  // Calibration: 0 to 26000 is a typical range for a 3.3V pot on ADS1115
  int ail_mapped = map(raw_ail, 0, 26000, 1000, 2000) + sticks.trim_ail;
  int ele_mapped = map(raw_ele, 0, 26000, 1000, 2000) + sticks.trim_ele;
  int rud_mapped = map(raw_rud, 0, 26000, 1000, 2000) + sticks.trim_rud;
  int thr_mapped = map(raw_thr, 0, 26000, 1000, 2000); // Usually no trim on throttle

  // Constrain outputs to legal RC range
  ail_mapped = constrain(ail_mapped, 800, 2200);
  ele_mapped = constrain(ele_mapped, 800, 2200);

  // 4. Update Display
  u8g2.clearBuffer();
  drawStatusHeader();
  drawStickMonitor(ail_mapped, ele_mapped);
  drawTelemetryFooter();
  u8g2.sendBuffer();

  delay(10); // ~100Hz loop rate
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
  // Draw Throttle Bar
  u8g2.drawStr(0, 30, "THR:");
  u8g2.drawFrame(30, 22, 40, 10);
  int thr_w = map(ads.readADC_SingleEnded(2), 0, 26000, 0, 38);
  u8g2.drawBox(31, 23, constrain(thr_w, 0, 38), 8);

  // Draw Ail/Ele Crosshair
  int cx = 100;
  int cy = 35;
  u8g2.drawFrame(cx-15, cy-15, 30, 30);
  u8g2.drawVLine(cx, cy-15, 30);
  u8g2.drawHLine(cx-15, cy, 30);
  
  // Draw stick position dot
  int dot_x = map(ail, 1000, 2000, cx-14, cx+14);
  int dot_y = map(ele, 1000, 2000, cy+14, cy-14);
  u8g2.drawDisc(constrain(dot_x, cx-14, cx+14), constrain(dot_y, cy-14, cy+14), 2);

  // Show Trim Values
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setCursor(0, 45);
  u8g2.print("T-AIL: "); u8g2.print(sticks.trim_ail);
}

void drawTelemetryFooter() {
  u8g2.drawHLine(0, 52, 128);
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.setCursor(0, 62);
  u8g2.print("PLANE: "); u8g2.print(sticks.plane_vbatt); u8g2.print("V");
  u8g2.drawStr(75, 62, "RATE: HIGH");
}

