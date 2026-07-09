# Trim Implementation: Mechanical vs. Digital

In radio control, "Trim" is used to fine-tune the neutral position of your control surfaces (ailerons, elevator, rudder) so the airplane flies straight without stick input. For your Kraft 7 conversion, you have two choices.

## 1. Option A: Mechanical Trim (The "Vintage" Way)

The original Kraft 7 uses mechanical levers that physically rotate the body of the stick potentiometers.

- **How it works**: When you move the small slider next to the stick, it physically shifts the pot. The ESP32 simply sees a different analog value at "center."
- **Wiring**: No extra wiring is needed.
- **Code**: No extra code is needed.
- **Pros**: Maintains the 100% original look and feel of the Kraft radio.
- **Cons**: No Model Memory. If you fly two different planes, you have to manually move the physical sliders every time you swap.

---

## 2. Option B: Digital Trim (The "Modern" Way)

In this approach, you ignore or remove the mechanical sliders and add small buttons (or 5-way navigation switches) to the case.

- **How it works**: The ESP32 maintains a `trim_value` variable in its memory for each axis.
- **Math**: `Final_Output = Raw_Stick_Value + Trim_Value`.
- **Pros**: 
    - **Model Memory**: The ESP32 can save different trim settings for 10+ different airplanes.
    - **Precision**: You can adjust trim in very small, repeatable increments.
    - **Audible Feedback**: You can program the ESP32 to "beep" when you center the trim.

---

## 3. Implementing Digital Trim in Code

To use Digital Trim, you need to add four momentary buttons (Up/Down for Ele, Left/Right for Ail/Rud).

### Logic Snippet:
```cpp
int trim_ail = 0; // Ranges from -100 to +100

void handleTrims() {
  if (digitalRead(TRIM_AIL_LEFT) == LOW) {
    trim_ail--;
    playBeep(); // Audio feedback
    saveToMemory(); // Save to ESP32 Flash/NVS
  }
  if (digitalRead(TRIM_AIL_RIGHT) == LOW) {
    trim_ail++;
    playBeep();
    saveToMemory();
  }
}

// Applying the trim
int final_aileron = raw_aileron + trim_ail;
```

## 4. Recommendation for ne9n
Since you are building a modern 50MHz digital link, **Option B (Digital Trim)** is the better choice. It allows you to take full advantage of the ESP32's memory. You can hide the buttons inside the case or mount them subtly near the original trim slots.

---
*Created for the Kraft 7 Digital Conversion Project.*
