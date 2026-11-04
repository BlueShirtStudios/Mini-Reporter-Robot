#include "BluetoothClient.h"

BluetoothClient picoClient(
    "19b10000-e8f2-537e-4f6c-d104768a1214", 
    "19b10001-e8f2-537e-4f6c-d104768a1214", 
    "PicoW_Controller"
);

void setup() {
  Serial.begin(115200);
  delay(1000);
  picoClient.begin();
}

void loop() {
  picoClient.update();
  delay(1000);
}