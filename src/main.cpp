#include <Arduino.h>
#include <NimBLEDevice.h>

NimBLEServer *pServer;
NimBLECharacteristic *pCharacteristic;

const int ledPin = 2;
const int buttonPin = 3;
const int leftBuzzerPin = 4;
const int rightBuzzerPin = 5;
int buttonState = 0;
int lastButtonState = 0;
int ledState = LOW;

class MyCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      char received = value[0];
      switch (received) {
        case 'A':
          digitalWrite(leftBuzzerPin, HIGH);
          break;
        case 'a':
          digitalWrite(leftBuzzerPin, LOW);
          break;
        case 'B':
          digitalWrite(rightBuzzerPin, HIGH);
          break;
        case 'b':
          digitalWrite(rightBuzzerPin, LOW);
          break;
        default:
          break;
      }
    }
  }
};

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(leftBuzzerPin, OUTPUT);
  pinMode(rightBuzzerPin, OUTPUT);

  NimBLEDevice::init("ESP32_BT");
  pServer = NimBLEDevice::createServer();
  NimBLEService *pService = pServer->createService("FF01");
  pCharacteristic = pService->createCharacteristic(
                     "FF02",
                     NIMBLE_PROPERTY::READ   |
                     NIMBLE_PROPERTY::WRITE  |
                     NIMBLE_PROPERTY::NOTIFY
                   );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  pServer->getAdvertising()->start();
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
  lastButtonState = buttonState;
  delay(10);
}