#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 BLE Client Initialized. Scanning for Pico W...");
  
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
}

void loop() {
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("Connected to the Pico W!");
    }
    doConnect = false;
  }

  if (!connected && !doConnect) {
    BLEDevice::getScan()->start(5, false);
  }
  delay(1000);
}