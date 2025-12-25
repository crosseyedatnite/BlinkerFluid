# Assembly & Installation Guide

Follow these steps to build your own Blinker Fluid Spine.

## 1. Wiring Diagram


### The "GND Bus" Strategy
To keep the spine thin enough to fit in the bottle neck:
1. Run one single wire (The "GND Bus") down the length of the spine.
2. Solder the negative leg of **every** component (LEDs, Buzzer, Button, Battery) to this one wire.

### The Voltage Divider (LDR)
- Connect one leg of the LDR to **5V**.
- Connect the other leg to **A0**.
- **Important:** Solder a 10kΩ resistor between **A0** and **GND** to complete the voltage divider.

## 2. Structural Assembly
1. **The Chassis:** Use a large (12"+) zip-tie as the backbone.
2. **Component Mounting:** - Hot glue the LDR to the very tip of the zip-tie.
   - Secure the Nano in the center of the tie.
   - Mount the 9V battery lower down to act as a weight/stabilizer.
3. **The Cap:** Hot glue the head of the zip-tie to the inside of your bottle cap. Ensure the LDR is centered to see light through the neck.

## 3. Handling the D4/D5 Bridge
In this specific build, Pins 4 and 5 are physically shorted. 
- **Hardware Fix:** Ensure both pins have 220Ω resistors.
- **Software Fix:** The provided code drives both pins `HIGH` or `LOW` simultaneously to prevent current contention.

## 4. Calibration
If your alarm triggers too easily, adjust the following variable in the code:
```cpp
int alarmThreshold = 500; // Increase this number for less sensitivity