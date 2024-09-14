#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
BLEServer *pServer=NULL;

void startAdvertising();
void showConnectedDeviceInfo();
class MyBLEServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      showConnectedDeviceInfo();
    };
   
    void onServerStart(BLEServer* pServer) {
      Serial.println("Server started!");
    }

    void onServerStop(BLEServer* pServer) {
      Serial.println("Server stopped!");
    }

    void onDisconnect(BLEServer* pServer) {
      startAdvertising();
    }

    void onCharacteristicWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }
        Serial.println();
        Serial.println("*********");
      }
    }
    void onCharacteristicRead(BLECharacteristic *pCharacteristic) {
      Serial.println("Characteristic read!");
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }
        Serial.println();
        Serial.println("*********");
      }
    }

    void onAdvertisingStart(BLEAdvertising* pAdvertising) {
      Serial.println("Advertising started!");
    }
    void onAdvertisingStop(BLEAdvertising* pAdvertising) {
      Serial.println("Advertising stopped!");
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Bluetooth Server....");
  BLEDevice::init("IdeaESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyBLEServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setValue("Hello World from IdeaESP32");
  pService->start();
  startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void startAdvertising()
{
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void showConnectedDeviceInfo()
{
  std::string s;
  
  for(int i=0;i<pServer->getConnectedCount();i++){
    s+="Connected Device: ";
    s+=pServer->getConnId();
    s+="\n";
  }
  Serial.println(s.c_str());
}
void loop() {
  if(Serial.available()){
    String s = Serial.readStringUntil('\n');
    Serial.println(s);
  }
  if(pServer->getConnectedCount()>0){
    pServer->getAdvertising()->stop();
  }else{
    startAdvertising();
  }
}

