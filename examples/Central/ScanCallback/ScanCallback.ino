/*
  Scan Callback

  This example scans for Bluetooth® Low Energy peripherals and prints out their advertising details:
  address, local name, advertised service UUIDs. Unlike the Scan example, it uses
  the callback style APIs and disables filtering so the peripheral discovery is
  reported for every single advertisement it makes.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

class MyServerCallbacks : public BLEDeviceCallbacks
{
  public:
    MyServerCallbacks(void *someClass)
      : _someClass(someClass)
    {
    }

  protected:
    void *_someClass = nullptr;

    void onConnect(BLEDevice device)
    {
      Serial.println("connected");
    };

    void onDisconnect(BLEDevice device)
    {
      Serial.println("disconnected");
    }

    void onDiscover(BLEDevice peripheral)
    {
      // discovered a peripheral
      Serial.println("Discovered a peripheral");
      Serial.println("-----------------------");

      // print address
      Serial.print("Address: ");
      Serial.println(peripheral.address());

      // print the local name, if present
      if (peripheral.hasLocalName()) {
        Serial.print("Local Name: ");
        Serial.println(peripheral.localName());
      }

      // print the advertised service UUIDs, if present
      if (peripheral.hasAdvertisedServiceUuid()) {
        Serial.print("Service UUIDs: ");
        for (int i = 0; i < peripheral.advertisedServiceUuidCount(); i++) {
          Serial.print(peripheral.advertisedServiceUuid(i));
          Serial.print(" ");
        }
        Serial.println();
      }

      // print the RSSI
      Serial.print("RSSI: ");
      Serial.println(peripheral.rssi());

      Serial.println();
    }
};

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  Serial.println("Bluetooth® Low Energy Central scan callback");

  // set the discovered event handle
  BLE.setCallbacks(new MyServerCallbacks(nullptr));

  // start scanning for peripherals with duplicates
  BLE.scan(true);
}

void loop() {
  // poll the central for events
  BLE.poll();
}
