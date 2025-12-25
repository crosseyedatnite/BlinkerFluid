#include "LowPower.h"

// --- Function Prototypes ---
void toggleSystem();
void runPoliceStrobe();
void allLedsOff();
void setLEDs(bool red, bool yellow, bool blue, bool white);
void playFeedback(bool active);

// --- Pin Definitions ---
const int ldrPin = A0;
const int piezoPin = 8;
const int buttonPin = 3; 
const int yellowLED = 4; // Shorted together
const int redLED    = 5; // Shorted together
const int blueLED   = 6; 
const int whiteLED  = 7;

// --- Thresholds ---
const int floorThreshold = 10;   
const int alarmThreshold = 250;

// --- Turn Signal (Red + Yellow) ---
const int TURN_SIGNAL_FREQ = 1500;
const int TURN_SIGNAL_TONE2_FREQ = 1200;
const int TURN_SIGNAL_TONE_DURATION = 20;
const int TURN_SIGNAL_BLINK_INTERVAL = 400;

// --- Police Strobe ---
const int STROBE_ITERATIONS = 5;
const int STROBE_RED_YELLOW_FREQ = 2500;
const int STROBE_BLUE_FREQ = 1800;
const int STROBE_TONE_DURATION = 40;
const int STROBE_FLASH_DELAY = 50;

// --- Feedback Tones ---
const int FEEDBACK_ON_FREQ = 3500;
const int FEEDBACK_ON_TONE_DURATION = 60;
const int FEEDBACK_ON_LED_DELAY = 100;
const int FEEDBACK_ON_DELAY_BETWEEN = 50;
const int FEEDBACK_OFF_FREQ = 800;
const int FEEDBACK_OFF_TONE_DURATION = 400;
const int FEEDBACK_OFF_DELAY = 400;

volatile bool buttonPressed = false; 
bool systemActive = true;  

void setup() {
  Serial.begin(9600);
  pinMode(piezoPin, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleSystem, FALLING);
}

void toggleSystem() {
  buttonPressed = true; 
}

void loop() {
  if (buttonPressed) {
    delay(50); 
    systemActive = !systemActive;
    playFeedback(systemActive);
    buttonPressed = false;
  }

  if (!systemActive) {
    allLedsOff();
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    return;
  }

  int lightLevel = analogRead(ldrPin);
  Serial.print("Light Level: "); Serial.println(lightLevel);

  if (lightLevel > alarmThreshold) {
    runPoliceStrobe();
  } else if (lightLevel > floorThreshold) {
    // --- STATE: TURN SIGNAL (Now using Red + Yellow) ---
    setLEDs(true, true, false, false);
    tone(piezoPin, TURN_SIGNAL_FREQ, TURN_SIGNAL_TONE_DURATION); 
    delay(TURN_SIGNAL_BLINK_INTERVAL);
    setLEDs(false, false, false, false);
    tone(piezoPin, TURN_SIGNAL_TONE2_FREQ, TURN_SIGNAL_TONE_DURATION);
    LowPower.powerDown(SLEEP_500MS, ADC_OFF, BOD_OFF);
  } else {
    allLedsOff();
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF); 
  }
}

void runPoliceStrobe() {
  for (int i = 0; i < STROBE_ITERATIONS; i++) {
    // Flash Red/Yellow/White together
    setLEDs(true, true, true, false);
    tone(piezoPin, STROBE_RED_YELLOW_FREQ, STROBE_TONE_DURATION);
    delay(STROBE_FLASH_DELAY);
    
    // Switch to Blue only
    setLEDs(false, false, true, false);
    tone(piezoPin, STROBE_BLUE_FREQ, STROBE_TONE_DURATION);
    delay(STROBE_FLASH_DELAY);
  }
  setLEDs(false, false, false, false);
}

void playFeedback(bool active) {
  if (active) {
    for(int i=0; i<2; i++) {
      tone(piezoPin, FEEDBACK_ON_FREQ, FEEDBACK_ON_TONE_DURATION);
      setLEDs(false, false, false, true);
      delay(FEEDBACK_ON_LED_DELAY);
      setLEDs(false, false, false, false);
      delay(FEEDBACK_ON_DELAY_BETWEEN);
    }
  } else {
    tone(piezoPin, FEEDBACK_OFF_FREQ, FEEDBACK_OFF_TONE_DURATION);
    setLEDs(true, true, false, false);
    delay(FEEDBACK_OFF_DELAY);
    setLEDs(false, false, false, false);
  }
}

void setLEDs(bool red, bool yellow, bool blue, bool white) {
  digitalWrite(redLED, red ? HIGH : LOW);
  digitalWrite(yellowLED, yellow ? HIGH : LOW);
  digitalWrite(blueLED, blue ? HIGH : LOW);
  digitalWrite(whiteLED, white ? HIGH : LOW);
}

void allLedsOff() {
  setLEDs(false, false, false, false);
  noTone(piezoPin);
}