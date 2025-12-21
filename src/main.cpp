#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);
void goAlarm();
void justBlink();
void clickySignal();
void turnSignal();
const int ldrPin = A0;
const int piezoPin = 8;
const int ledPin = 13;
int lightThreshold = 200; // Adjust this based on your bottle's frostiness


void setup() {
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600); // Use this to calibrate your threshold
}

void loop() {
int lightLevel = analogRead(ldrPin);
  return;;
  // Uncomment the line below to see your light values in the Serial Monitor
   Serial.println(lightLevel);

  if (lightLevel > lightThreshold) {
    // CAP REMOVED: Alarm Mode!
    goAlarm();
  } else {
    // CAP ON: Turn Signal Mode
    turnSignal();
  }
}

void turnSignal()
{
  digitalWrite(LED_BUILTIN, HIGH); // Light on
  tone(piezoPin, 1500, 15);   // Tick!
  delay(400);
  
  digitalWrite(LED_BUILTIN, LOW);  // Light off
  tone(piezoPin, 1200, 15);   // Tock!
  delay(400);
}

void clickySignal()
{
  // The "Tick"
  tone(piezoPin, 1500); // High pitch for sharpness
  delay(15);            // Very short duration
  noTone(piezoPin);     // Stop the sound
  
  delay(400);           // Gap between tick and tock
  
  // The "Tock" (slightly lower pitch for realism)
  tone(piezoPin, 1200); 
  delay(15);
  noTone(piezoPin);
  
  delay(400);           // Gap before the next cycle
}

void goAlarm()
{
  // Rapid Warble (0.5 seconds)
  for(int i=0; i<5; i++) {
    tone(8, 600, 100);
  digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
  
  // Fast Rise (0.5 seconds)
  for(int i=500; i<2000; i+=20) {
    tone(8, i, 10);
    delay(10);
  }
}

