#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <vector>
#include <HardwareData.h>

class ClientHandler : public BLECharacteristicCallbacks {
public:
    ClientHandler() {
		Serial.begin(115200);
		BLEDevice::init("CYD_Server");
		BLEServer *pServer = BLEDevice::createServer();
		BLEService *pService = pServer->createService("12345678-1234-1234-1234-1234567890ab");
		pCharacteristic = pService->createCharacteristic(
			"87654321-4321-4321-4321-0987654321ba",
			BLECharacteristic::PROPERTY_WRITE
		);

		pCharacteristic->setCallbacks(this);
		pService->start();

		BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
		pAdvertising->setScanResponse(true);
		pAdvertising->addServiceUUID("12345678-1234-1234-1234-1234567890ab");
		pAdvertising->start();

		Serial.println("Server is running...");
	}
    
	void onWrite(BLECharacteristic *pCharacteristic) {
		std::string receivedData = pCharacteristic->getValue();
		Serial.print("Received: ");
		Serial.println(receivedData.c_str());
		hardwareData = receivedData.c_str();
	}

	String getHardwareData() {
		return hardwareData;
	}

private:
	BLECharacteristic *pCharacteristic;
	String hardwareData = "";
};