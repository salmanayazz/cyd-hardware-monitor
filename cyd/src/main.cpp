#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();   
BLECharacteristic *pCharacteristic;

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string receivedData = pCharacteristic->getValue();
        Serial.print("Received: ");
        Serial.println(receivedData.c_str());

        tft.println(receivedData.c_str());
    }
};

void setup() {
    // setup display
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.setRotation(1);
    tft.setTextFont(1);
    tft.setTextSize(2);
    tft.setTextDatum(TL_DATUM);

    // setup BLE
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

    Serial.println("Server is running...");
}

void loop() {}
