#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <vector>

#define SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-0987654321ba"

class ClientHandler : public BLECharacteristicCallbacks {
public:
    ClientHandler() {
        Serial.begin(115200);
        BLEDevice::init("CYD_Server");

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

        // place the received data into string array
        std::vector<std::string> data;
        std::string temp = "";
        for (char c : receivedData) {
            if (c == ',') {
                data.push_back(temp);
                temp = "";
            } else {
                temp += c;
            }
        }
        data.push_back(temp);

        // convert string array to integer array
        std::vector<int> intData;
        for (std::string s : data) {
            intData.push_back(std::stoi(s));
        }

        hardwareData.cpuUsage.push_back(intData[0]);
        hardwareData.cpuTemp.push_back(intData[1]);
        hardwareData.gpuUsage.push_back(intData[2]);
        hardwareData.gpuTemp.push_back(intData[3]);
        hardwareData.ramUsage.push_back(intData[4]);
    }

    HardwareData getHardwareData() {
        return hardwareData;
    }

    void startAdvertising() {
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->setScanResponse(true);
		pAdvertising->setMinPreferred(0x06);
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
    HardwareData hardwareData = HardwareData();

    class ServerCallbacks : public BLEServerCallbacks {
    public:
        ServerCallbacks(ClientHandler *handler) : handler(handler) {}

        void onConnect(BLEServer *pServer) override {
            Serial.println("Client connected");
        }

        void onDisconnect(BLEServer *pServer) override {
            Serial.println("Client disconnected");
            handler->restartAdvertising();
        }

    private:
        ClientHandler *handler;
    };
};
