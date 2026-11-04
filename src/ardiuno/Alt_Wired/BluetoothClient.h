#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>

class BluetoothClient
{
private:
    //Configuration for ble client
    BLEUUID serviceUUID;
    BLEUUID charUUID;
    String targetDevice;

    //State tracking
    bool doConnect;
    bool connected;
    BLEAdvertisedDevice* myDevice;
    BLERemoteCharacteristic* pRemoteCharacteristic;

    //Nested to class used to mimic events
    class ClientCallback : public BLEClientCallbacks 
    {
        BluetoothClient* parent;
    public:
        ClientCallback(BluetoothClient* p);
        void onConnect(BLEClient* pclient) override;
        void onDisconnect(BLEClient* pclient) override;
    };

    //Nested class for scanning and filtering devices
    class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks 
    {
        BluetoothClient* parent;
    public:
        AdvertisedDeviceCallbacks(BluetoothClient* p);
        void onResult(BLEAdvertisedDevice advertisedDevice) override;
    };

    ClientCallback clientCallback;
    AdvertisedDeviceCallbacks advCallbacks;

    //Static callback method that fires when new data is received
    static void notifyCallback(BLERemoteCharacteristic* pChar, uint8_t* pData, size_t length, bool isNotify);

    //Helper to establish the link
    bool connectToServer();

public:
    BluetoothClient(const char* serviceUuid, const char* charUuid, const char* deviceName);
    void begin();
    void update();
    bool isConnected() const;
};