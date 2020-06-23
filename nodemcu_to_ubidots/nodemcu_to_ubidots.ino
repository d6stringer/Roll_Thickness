#include "Ubidots.h"
const char* UBIDOTS_TOKEN = "BBFF-4TkMCRDQEvpWBRSsuV0Zc4pV3NDrEi";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "BayMaterials48480";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "6505660800";      // Put here your Wi-Fi password 

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  ubidots.setDebug(true);

}

void loop() {
  float value1 = random(0, 9) * 10;
  
  ubidots.add("this-is-a-number", value1);// Change for your variable name  
  
  bool bufferSent = false;
  bufferSent = ubidots.send(); // Will send data to a device label that matches the device Id

  if (bufferSent) {
  // Do something if values were sent properly
   Serial.println("Values sent by the device");
  }
  delay(5000);

}
