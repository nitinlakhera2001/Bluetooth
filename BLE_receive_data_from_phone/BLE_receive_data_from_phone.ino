#include<BLEDevice.h>
#include<BLEServer.h>
#include<BLEUtils.h>
#include<BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
int txValue = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID_TX "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID_RX "beb5483d-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
      deviceConnected = true;
    }
    void onDisconnect(BLEServer *pServer)
    {
      deviceConnected = false;
    }
};

class MyCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic)
    {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0)
      {
        Serial.println("=========START_RECEIVE========");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.println(rxValue[i]);
      }
    }
};
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
 /////////////////////////////////////////////
  BLEDevice :: init("ESP32");
  BLEServer *pServer = BLEDevice :: createServer();
  pServer-> setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer -> createService(SERVICE_UUID);
  pCharacteristic = pService -> createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic :: PROPERTY_NOTIFY);
  pCharacteristic -> addDescriptor( new BLE2902());
  ////////////////////////////////////////////////
  //characteristic for receiving end
  BLECharacteristic *pCharacteristic = pService ->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic -> setCallbacks(new MyCallback());
  
  pService -> start();
  pServer -> getAdvertising() -> start();
  Serial.println(" waiting for a client connection to notify...");


}
int i = 0;
void loop() {
  Serial.print("Device status: ");Serial.println(deviceConnected);
  if (deviceConnected)
  {
    int txValue = i++;
    char txString[8];
    dtostrf(txValue, 1, 2, txString);
    pCharacteristic->setValue(txString);
    pCharacteristic->notify();
    Serial.println("sent value" + String(txString));
    delay(500);

  }

  // put your main code here, to run repeatedly:

}
