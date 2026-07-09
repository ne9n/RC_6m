# Kraft 7: Case Modification & Mechanical Integration

This guide details how to modify the vintage Kraft 7 gold-anodized aluminum case to accommodate modern digital components while preserving its classic aesthetic.

## 1. Component Placement Strategy

| Component | Recommended Location | Installation Method |
| :--- | :--- | :--- |
| **1.3" OLED** | Top Center (above the Kraft Logo) | Rectangular cutout; flush mount with 3D printed bezel. |
| **USB-C Port** | Bottom Plate (original charge jack location) | Step-drill to 10mm; mount panel-mount USB-C extension. |
| **ESP32-S3 Deck** | Internal Center | Mount on 3mm standoff plate attached to rear case half. |
| **50MHz Antenna** | Top Right (original location) | Standard SMA-to-stud bulkhead adapter. |
| **LiPo Battery** | Original Battery Bay | Secure with high-density foam and velcro strap. |

---

## 2. The "Digital Deck" (Internal Sub-Chassis)
To avoid drilling excessive holes in the original case, create a **3D printed or FR4 "Digital Deck"** that mounts to the existing joystick assembly screws.

- **Stackup**:
    1. Bottom: 2S LiPo.
    2. Middle: ADS1115 and Power Distribution Board.
    3. Top: ESP32-S3 and 1W Power Amplifier (needs clear airflow).

---

## 3. Cutting the Aluminum Case

### 3.1 OLED Cutout
- **Tools**: Dremel with reinforced cutoff wheel, small needle files.
- **Dimensions**: 35mm x 18mm for a 1.3" SH1106 display.
- **Tip**: Apply blue painter's tape to the face of the radio before cutting to prevent scratches to the gold anodizing.

### 3.2 USB-C Integration
- Remove the old round DIN charge jack.
- Use a **USB-C Panel Mount Cable** (short 10cm version) to bridge from the internal charger to the exterior. This allows you to charge without opening the case.

---

## 4. Mechanical Preservation Rules
- **DO NOT** remove the original mechanical trim levers. We will disconnect their internal linkage but keep the external levers for "fidget" and aesthetic value.
- **DO NOT** paint the case. The original Kraft Gold is iconic.
- **WIRING**: Use braided cable sleeves for a "period-correct" look for any wires visible through the back or battery door.

---

## 5. Heat Management (1W PA)
The **RD01MUS2** transistor will generate heat during long flights.
- Ensure the PA is mounted on a small copper heat spreader.
- Solder the thermal tab of the SOT-89 package to a large ground plane area on your PCB.

---
*Created for the Kraft 7 "Restomod" Project.*
