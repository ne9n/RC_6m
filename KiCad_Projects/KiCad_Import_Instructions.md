# How to Import this Project into KiCad

I have generated the core **Netlist (.net)** files for both the Transmitter and Receiver. These files contain all the electrical connections (nets) and footprint assignments.

## 🛠️ Step 1: Create a New Project in KiCad
1.  Open KiCad and create a new project named `RX_50MHz_SDR` or `TX_50MHz_1W`.
2.  Place the `.net` file I generated into that project folder.

## ⚡ Step 2: Import the Schematic
1.  Open the **Schematic Editor** (`.kicad_sch`).
2.  Go to **File > Import > Netlist...**
3.  Select the corresponding `.net` file.
4.  KiCad will automatically populate the schematic with all the components and their connections.

## 📏 Step 3: Start the PCB Layout
1.  Open the **PCB Editor** (`.kicad_pcb`).
2.  Go to **Tools > Update PCB from Schematic...**
3.  All footprints will appear, pre-wired with "ratsnest" lines showing the connections defined in the Netlist.

---

## 🏗️ RX Board Layout Concept (Copy into KiCad PCB Editor)
To help you start the layout, I have calculated the **Board Outline (30x50mm)**. You can draw this on the `Edge.Cuts` layer:

- **Top Left**: (0, 0)
- **Top Right**: (50, 0)
- **Bottom Right**: (50, 30)
- **Bottom Left**: (0, 30)

### Placement Zones:
1.  **RF Front End**: Keep U3 (LNA) and U2 (Mixer) in the top-left corner.
2.  **MCU**: Place U1 (ESP32) in the center-right.
3.  **IO**: Place J1-J8 (Servo Headers) along the bottom edge for easy wiring.

---

## 🏗️ TX Board Layout Concept
- **Board Outline**: **60x80mm** (To fit the Kraft battery bay or internal deck).
- **PA Section**: Place Q1 (RD01MUS2) near the top SMA connector with a large ground pour for cooling.
- **Sticks**: Place U3/U4 (ADCs) near the joystick assembly for short analog wires.

---
*Files generated at: \KiCad_Projects\*
