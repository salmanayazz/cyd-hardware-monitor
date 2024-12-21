#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLECharacteristic *pCharacteristic;
class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string receivedData = pCharacteristic->getValue();
        Serial.print("Received: ");
        Serial.println(receivedData.c_str());
    }
};

void setup() {
    Serial.begin(115200);
    BLEDevice::init("CYD_Server");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService("12345678-1234-1234-1234-1234567890ab");
    pCharacteristic = pService->createCharacteristic(
        "87654321-4321-4321-4321-0987654321ba",
        BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setCallbacks(new MyCallbacks());
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->setScanResponse(true);
    pAdvertising->addServiceUUID("12345678-1234-1234-1234-1234567890ab");
    pAdvertising->start();

    Serial.println("BLE Server is running...");
}

void loop() {
    
}
