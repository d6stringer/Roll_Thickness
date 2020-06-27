#include <ESP8266WiFi.h>

volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
uint8_t ENCODER_A = D5;
uint8_t ENCODER_B = D6;

void setup() {
  Serial.begin (9600);

  pinMode(ENCODER_A, INPUT_PULLUP); // internal pullup input pin 2

  pinMode(ENCODER_B, INPUT_PULLUP); // internalเป็น pullup input pin 3
  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), ai0, RISING);

  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), ai1, RISING);
}

void loop() {
  // Send the value of counter
  if ( counter != temp ) {
    Serial.println (counter);
    temp = counter;
  }
}

ICACHE_RAM_ATTR void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if (digitalRead(ENCODER_B) == LOW) {
    counter++;
  } else {
    counter--;
  }
}

ICACHE_RAM_ATTR void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if (digitalRead(ENCODER_A) == LOW) {
    counter--;
  } else {
    counter++;
  }
}
