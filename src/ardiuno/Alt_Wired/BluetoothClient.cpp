#include "BluetoothClient.h"

//Constructor using the member intializer
BluetoothClient::BluetoothClient(const char* serviceUuid, const char* charUuid, const char* deviceName)
    : serviceUUID(serviceUuid), charUUID(charUuid), targetDevice(deviceName),
      doConnect(false), connected(false), myDevice(nullptr), pRemoteCharacteristic(nullptr),
      clientCallback(this), advCallbacks(this) {}

void BluetoothClient::begin() 
{
  //Starts the bluetooth connection
    BLEDevice::init("");
    Serial.println("ESP32 BLE Client Initialized. Scanning...");

    //Scans for the Pico 
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(&advCallbacks);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
}

void BluetoothClient::update() {
  //Driver for the connection
    if (doConnect) {
        if (connectToServer()) {
          //If it is
            Serial.println("Connected to the Pico W Controller!");
        }
        doConnect = false;
    }

    //Else it will search again
    if (!connected && !doConnect) {
        BLEDevice::getScan()->start(5, false);
    }
}

bool BluetoothClient::isConnected() const {
    return connected;
}

bool BluetoothClient::connectToServer() {
    Serial.print("Connecting to: ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    //Creates a bluetooth client to bind to our callback
    BLEClient* pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(&clientCallback);


    //Try to connect to the controller
    pClient->connect(myDevice);

    //Query available devices with the UUID we have
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) return false;

    //Query for the service UUID we have
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) return false;

    //If we can send notifications, register our hook
    if (pRemoteCharacteristic->canNotify())
        pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

void BluetoothClient::notifyCallback(BLERemoteCharacteristic* pChar, uint8_t* pData, size_t length, bool isNotify) 
{
    //This runs as soon as a packet is recieved from the controller
    Serial.print("Received data from Pico W: ");
    for (size_t i = 0; i < length; i++) {
        Serial.print((char)pData[i]);
    }
    Serial.println();
}

BluetoothClient::ClientCallback::ClientCallback(BluetoothClient* p) : parent(p) {}
void BluetoothClient::ClientCallback::onConnect(BLEClient* pclient) {}
void BluetoothClient::ClientCallback::onDisconnect(BLEClient* pclient) 
{
    parent->connected = false;
    Serial.println("Disconnected from Pico W");
}

BluetoothClient::AdvertisedDeviceCallbacks::AdvertisedDeviceCallbacks(BluetoothClient* p) : parent(p) {}
void BluetoothClient::AdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) 
{
    //
    if (advertisedDevice.haveName() && advertisedDevice.getName() == parent->targetDevice.c_str()) {
        BLEDevice::getScan()->stop();
        parent->myDevice = new BLEAdvertisedDevice(advertisedDevice);
        parent->doConnect = true;
    }
}