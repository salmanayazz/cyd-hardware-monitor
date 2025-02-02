#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <vector>
#include <ArduinoJson.h>

#define SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-0987654321ba"

class ClientHandler : public BLECharacteristicCallbacks {
public:
    ClientHandler() {
        Serial.begin(115200);
        BLEDevice::init("Hardware_Monitor_Server");

        pServer = BLEDevice::createServer();
        pServer->setCallbacks(new ServerCallbacks(this));

        BLEService *pService = pServer->createService(SERVICE_UUID);

        pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_WRITE | 
			BLECharacteristic::PROPERTY_READ | 
			BLECharacteristic::PROPERTY_NOTIFY
        );

        pCharacteristic->setCallbacks(this);
        pService->start();

        startAdvertising();
        Serial.println("Server is running...");
    }
    
    void onWrite(BLECharacteristic *pCharacteristic) override {
        std::string receivedData = pCharacteristic->getValue();
        Serial.print("Received: ");
        Serial.println(receivedData.c_str());

        StaticJsonDocument<300> doc;
        DeserializationError error = deserializeJson(doc, receivedData.c_str());
        
        if (error) {
            Serial.print("Deserialization failed: ");
            Serial.println(error.c_str());
            return;
        }


        if (doc["isPrimary"]) {
            clientDataList[0].cpuUtil.push_back(doc["cpuUtil"]);
            clientDataList[0].cpuTemp.push_back(doc["cpuTemp"]);
            clientDataList[0].gpuUtil.push_back(doc["gpuUtil"]);
            clientDataList[0].gpuTemp.push_back(doc["gpuTemp"]);
            clientDataList[0].memoryUtil.push_back(doc["memoryUtil"]);
            clientDataList[0].fps.push_back(doc["fps"]);
            clientDataList[0].fps.setTitle("FPS - " + doc["fpsProcess"].as<String>());
        } else {
            clientDataList[1].cpuUtil.push_back(doc["cpuUtil"]);
            clientDataList[1].cpuTemp.push_back(doc["cpuTemp"]);
            clientDataList[1].memoryUtil.push_back(doc["memoryUtil"]);
        }
        
    }

    ClientData* getClientDataList() {
        return clientDataList;
    }

    void startAdvertising() {
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->setScanResponse(true);
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->start();
        Serial.println("Advertising started");
    }

    void restartAdvertising() {
        Serial.println("Restarting advertising");
        pServer->startAdvertising();
    }

private:
    BLECharacteristic *pCharacteristic;
    BLEServer *pServer;
    ClientData clientDataList[2] = {ClientData(), ClientData()};

    class ServerCallbacks : public BLEServerCallbacks {
    public:
        ServerCallbacks(ClientHandler *handler) : handler(handler) {}

        void onConnect(BLEServer *pServer) override {
            Serial.println("Client connected");
            handler->restartAdvertising();
        }

        void onDisconnect(BLEServer *pServer) override {
            Serial.println("Client disconnected");
            handler->restartAdvertising();
        }

    private:
        ClientHandler *handler;
    };
};