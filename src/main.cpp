#include "LowPower.h"

// Pin Definitions
const int ldrPin = A0;
const int piezoPin = 8;
const int buttonPin = 3; 
const int yellowLED = 4; // Turn Signal
const int redLED    = 5; // Alarm Strobe / Disarmed indicator
const int blueLED   = 6; // Alarm Strobe
const int whiteLED  = 7; // Alarm Strobe / Armed indicator

volatile bool systemActive = true; 
void toggleSystem();
void runPoliceStrobe();
void allLedsOff();

// --- CALIBRATION ---
int floorThreshold = 50;   
int alarmThreshold = 500;  

void setup() {
  Serial.begin(9600);

  Serial.println("System initialized.");
  pinMode(piezoPin, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Attach interrupt to wake up or arm/disarm
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleSystem, FALLING);
}

void toggleSystem() {
  systemActive = !systemActive;
  
  if (systemActive) {
    // Armed Chirp-Chirp
    for(int i=0; i<2; i++) {
      tone(piezoPin, 3000, 50);
      digitalWrite(whiteLED, HIGH);
      delay(70);
      noTone(piezoPin);
      digitalWrite(whiteLED, LOW);
      delay(50);
    }
  } else {
    // Disarmed Long Chirp
    tone(piezoPin, 1000, 200);
    digitalWrite(redLED, HIGH);
    delay(200);
    noTone(piezoPin);
    digitalWrite(redLED, LOW);
  }
}

void loop() {
  // If system is deactivated, sleep forever to save battery
  if (!systemActive) {
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    return;
  }

  int lightLevel = analogRead(ldrPin);
  
  // Calibration output
  Serial.print("Light Level: "); Serial.println(lightLevel);

  if (lightLevel > alarmThreshold) {
    // --- STATE 3: ALARM ---
    runPoliceStrobe();
  } else if (lightLevel > floorThreshold) {
    // --- STATE 2: TURN SIGNAL ---
    digitalWrite(yellowLED, HIGH);
    tone(piezoPin, 1500, 15); 
    delay(400);
    digitalWrite(yellowLED, LOW);
    tone(piezoPin, 1200, 15);
    LowPower.powerDown(SLEEP_500MS, ADC_OFF, BOD_OFF);
  } else {
    // --- STATE 1: STEALTH ---
    allLedsOff();
    delay(200);
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF); 
  }
}

void runPoliceStrobe() {
  digitalWrite(yellowLED, LOW);
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLED, HIGH);
    digitalWrite(blueLED, LOW);
    digitalWrite(whiteLED, HIGH);
    tone(piezoPin, 2500, 40);
    delay(50);
    
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, HIGH);
    digitalWrite(whiteLED, LOW);
    tone(piezoPin, 1800, 40);
    delay(50);
  }
  digitalWrite(blueLED, LOW);

}

void allLedsOff() {
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(whiteLED, LOW);
  noTone(piezoPin);
}