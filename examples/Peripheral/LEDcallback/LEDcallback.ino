/*
  Callback LED

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED. The callback features of the
  library are used.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin to use for the LED

class MyServerCallbacks : public BLEDeviceCallbacks
{
  public:
    MyServerCallbacks(void* someClass)
      : _someClass(someClass)
    {
    }

  protected:
    void* _someClass = nullptr;

    void onConnect(BLEDevice central)
    {
      // central connected event handler
      Serial.print("Connected event, central: ");
      Serial.println(central.address());
    };

    void onDisconnect(BLEDevice central)
    {
      // central disconnected event handler
      Serial.print("Disconnected event, central: ");
      Serial.println(central.address());
    }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
{
  public:
    MyCharacteristicCallbacks(void *someClass)
      : _someClass(someClass)
    {
    }

  protected:
    void* _someClass = nullptr;

    void onWrite(BLECharacteristic switchCharacteristic)
    {
      // central wrote new value to characteristic, update LED
      Serial.print("Characteristic event, written: ");

      if (switchCharacteristic.value())
      {
        Serial.println("LED on");
        digitalWrite(ledPin, HIGH);
      }
      else
      {
        Serial.println("LED off");
        digitalWrite(ledPin, LOW);
      }
    }

    void onRead(BLECharacteristic characteristic)
    {
    }
};

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  pinMode(ledPin, OUTPUT); // use the LED pin as an output

  // begin initialization
  if (!BLE.begin())
  {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1)
      ;
  }

  // set the local name peripheral advertises
  BLE.setLocalName("LEDCallback");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setCallbacks(new MyServerCallbacks(nullptr));

  // assign callback for characteristic
  switchCharacteristic.setCallbacks(new MyCharacteristicCallbacks(nullptr));
  // set an initial value for the characteristic
  switchCharacteristic.setValue(0);

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));
}

void loop()
{
  // poll for Bluetooth® Low Energy events
  BLE.poll();
}
