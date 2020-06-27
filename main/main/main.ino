
//NodeMCU example: https://help.ubidots.com/en/articles/513312-connect-a-nodemcu-esp8266-to-ubidots-over-http
//digimatic reader: https://github.com/tlbruns/Digimatic

#include <Digimatic.h>
#include "math.h"
#include "secrets.h"
#include "Ubidots.h"


// define indicator pins
uint8_t req_pin = 4;
uint8_t data_pin = 0;
uint8_t clk_pin = 5;

//define encoder vars
volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
uint8_t ENCODER_A = D5;
uint8_t ENCODER_B = D6;

//physical encoder info
const int rotor = 47640; //rotor size in um
const int encoder_steps = 360; //encoder pulses per revolution
const int measure_len = 50000; //length between recorded measurements in um
const float rotor_circ = rotor * M_PI; //circumference of rotor in um
const int step_len = round(rotor_circ / encoder_steps); //arc length of 1 encoder step rounded to nearest um
const int steps_btwn_measure = round(measure_len / step_len); //number of steps between measurement

// wifi info (stored in secrets.h)
extern const char* UBIDOTS_TOKEN;
extern const char* WIFI_SSID;
extern const char* WIFI_PASS;


// initialize Digimatic
Digimatic indicator = Digimatic(clk_pin, data_pin, req_pin);

// initialize Ubidots
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);


void setup()
{
  Serial.begin(115200);
  // initialize Ubidots
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  //ubidots.setDebug(true);

  //encoder pin setup
  pinMode(ENCODER_A, INPUT_PULLUP); // internal pullup input
  pinMode(ENCODER_B, INPUT_PULLUP); // internal pullup input
  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), ai0, RISING);

  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), ai1, RISING);

}

void loop()
{
  //get measurement once encoder reaches measurement length
  float curr_measurement;
  int cur_pos = encoder();
  Serial.println(cur_pos);
  if (encoder() >= steps_btwn_measure) {
    // request current measurement
    curr_measurement = indicator.fetch();
    Serial.print(curr_measurement, indicator.decimal_places());
  }

  //prepare ubidots mesasge
  if (indicator.units_mm()) {
    ubidots.add("this-is-a-number", curr_measurement);
  }
  else {
    ubidots.add("this-is-a-number", curr_measurement * 25.4);
  }
  bool bufferSent = false;
  bufferSent = ubidots.send(); //send data to ubidots
  //reset encoder if data is sent
  if (bufferSent) {
    //encoder_reset();
    Serial.println (curr_measurement);
  }
  delay(5000) ;
}

int encoder()
{
  if ( counter != temp ) {
    Serial.println (counter);
    temp = counter;
  }
  return counter;
}

void encoder_reset() {
  counter = 0;
}

//Not exactly sure what ICACHE_RAM_ATTR does but it's required for attachInterrupt on the NodeMCU
ICACHE_RAM_ATTR void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if (digitalRead(3) == LOW) {
    counter++;
  } else {
    counter--;
  }
}

ICACHE_RAM_ATTR void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if (digitalRead(2) == LOW) {
    counter--;
  } else {
    counter++;
  }
}
