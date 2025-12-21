#include "LowPower.h"

const int ldrPin = A0;
const int piezoPin = 8;
const int buttonPin = 3; 
const int blinkerLed = 4;

volatile bool systemActive = true; // Set to true for calibration

// --- CALIBRATION VALUES (Update these after testing) ---
int floorThreshold = 50;   
int alarmThreshold = 300;  

void toggleSystem() {
  systemActive = !systemActive;
}

void setup() {
  Serial.begin(9600); // Start Serial communication
  pinMode(piezoPin, OUTPUT);
  pinMode(blinkerLed, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleSystem, FALLING);
  
  Serial.println("--- Blinker Fluid Calibration Mode ---");
}

void loop() {
  int lightLevel = analogRead(ldrPin);
  
  // PRINT CURRENT LEVEL
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  if (!systemActive) {
    Serial.println("System Sleep (Simulated)");
    delay(1000); 
    return;
  }

  if (lightLevel > alarmThreshold) {
    // --- STATE 3: ALARM ---
    digitalWrite(blinkerLed, HIGH);
    tone(piezoPin, 2000, 50);
    delay(50);
    digitalWrite(blinkerLed, LOW);
    tone(piezoPin, 1000, 50);
    delay(50);

  } else if (lightLevel > floorThreshold) {
    // --- STATE 2: TURN SIGNAL ---
    digitalWrite(blinkerLed, HIGH);
    tone(piezoPin, 1500, 15); 
    delay(400);
    digitalWrite(blinkerLed, LOW);
    tone(piezoPin, 1200, 15);
    
    delay(500); // Using delay instead of LowPower for calibration
    // Quick nap between ticks
    //LowPower.powerDown(SLEEP_500MS, ADC_OFF, BOD_OFF);

  } else {
    // --- STATE 1: STEALTH ---
    digitalWrite(blinkerLed, LOW);
    noTone(piezoPin);

    delay(1000); // Using delay instead of LowPower for calibration

    // Long nap because it's dark and nobody is looking
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  }
}