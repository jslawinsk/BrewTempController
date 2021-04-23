/*
 * Bas on Tech - Rotary Encoder and Interrupts
 * This course is part of the courses on https://arduino-lessons.com
 *  
 * (c) Copyright 2018-2019 - Bas van Dijk / Bas on Tech
 * This code and course is copyrighted. It is not allowed to use these courses commerically
 * without explicit written approval
 * 
 * YouTube:    https://www.youtube.com/c/BasOnTech
 * Facebook:   https://www.facebook.com/BasOnTechChannel
 * Instagram:  https://www.instagram.com/BasOnTech
 * Twitter:    https://twitter.com/BasOnTech
 * 
 * ---------------------------------------------------------------------------
 * 
 * More info about the Rotary Encoder: 
 * https://en.wikipedia.org/wiki/Rotary_encoder
 * 
 * PIN CONNECTIONS
 *
 * GND --> GND black
 *  +  --> 5V  red
 * SW  --> 12  yellow
 * DT  --> 3   green (data)
 * CLK --> 4   blue (clock)
 *
 */
//
// Define the following if using a Lolin D1 Mini
//
#define D1_MINI 1

#ifdef D1_MINI
  int pinB = D0;        // Connected to DT on KY-040
  int pinA = D5;       // Connected to CLK on KY-040
  int switchPin = D3;     // Should be 12
#else
  int pinB = 3;        // Connected to DT on KY-040
  int pinA = 10;       // Connected to CLK on KY-040
  int switchPin = 12;     // Should be 12
#endif

int switchState = HIGH;                    // button value
int pinAstateCurrent = LOW;                // Current state of Pin A
int pinAStateLast = pinAstateCurrent;      // Last read value of Pin A


void setup() {
  Serial.begin (9600);                     // Initialise the serial monitor

  pinMode (switchPin, INPUT_PULLUP);       // Enable the switchPin as input with a PULLUP resistor
    
  pinMode (pinA, INPUT);                   // Set PinA as input
  pinMode (pinB, INPUT);                   // Set PinB as input

  // Atach a CHANGE interrupt to PinB and exectute the update function when this change occurs.
 attachInterrupt(digitalPinToInterrupt(pinA), rotaryIsr, CHANGE);
 attachInterrupt(digitalPinToInterrupt(switchPin), buttonIsr, CHANGE);

}

void loop() {    
}

#ifdef D1_MINI
ICACHE_RAM_ATTR void buttonIsr() {
#else
void buttonIsr() {
#endif
  // BUTTON
  switchState = digitalRead(switchPin);    // Read the digital value of the switch (LOW/HIGH)
  // If the switch is pressed (LOW), print message
  if (switchState == LOW) {
      Serial.println("Switch pressed");
  }
}

#ifdef D1_MINI
ICACHE_RAM_ATTR void rotaryIsr() {
#else
void rotaryIsr() {
#endif
  /* WARNING: For this example I've used Serial.println within the interrupt callback. The Serial 
   * library already uses interrupts which could cause errors. Therefore do not use functions 
   * of the Serial libray in your interrupt callback.
   */
  // ROTATION DIRECTION
  pinAstateCurrent = digitalRead(pinA);    // Read the current state of Pin A
  
  // If there is a minimal movement of 1 step
  if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {
    if (digitalRead(pinB) == HIGH) {      // If Pin B is HIGH
      Serial.println("Counter Clockwise");             // Print on screen
    } else {
      Serial.println("Clockwise");            // Print on screen
    }
  }
  
  pinAStateLast = pinAstateCurrent;        // Store the latest read value in the currect state variable
  
}
