# Blinker Fluid Security Spine 🚨

An Arduino-powered prank and security device designed to fit inside a prop "Blinker Fluid" bottle. It uses an optical sensor to detect when the bottle is being handled or when the cap is removed, triggering a variety of light and sound responses.

## 🛠 Features
- **Stealth Mode:** Deep sleep functionality to preserve 9V battery life when the bottle is in total darkness.
- **Blinker Mode:** Simulates a mechanical turn signal (Yellow/Red) when ambient light is detected.
- **Police Alarm:** High-intensity strobing (Red, Blue, White) and a 100dB piezo siren if the cap is removed.
- **Interrupt Control:** A physical button toggle with audio feedback (Chirps) for Arming/Disarming.

## 💻 Software Logic
The system uses a state machine driven by an LDR (Light Dependent Resistor):
1. **Darkness (<50):** System sleeps for 4 seconds, wakes up to check light, then goes back to sleep.
2. **Ambient (50-500):** System blinks Yellow/Red and plays a "ticking" sound.
3. **Open Cap (>500):** System triggers a non-blocking police strobe and siren.

## 🔌 Hardware Configuration
- **Microcontroller:** Arduino Nano
- **Power:** 9V Battery via VIN
- **Pins Used:**
  - `A0`: LDR (Photoresistor)
  - `D3`: Arming Button (Interrupt)
  - `D4/D5`: Yellow & Red LEDs (Note: Tied together in software due to hardware bridge)
  - `D6`: Blue LED
  - `D7`: White LED
  - `D8`: Piezo Buzzer