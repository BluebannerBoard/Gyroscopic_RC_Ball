#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
String bpress = "B0:D\n";
// set motors
#define PIN_IN1  19 // ESP32 pin GPIO19 connected to the IN1 pin L298N
#define PIN_IN2  18 // ESP32 pin GPIO18 connected to the IN2 pin L298N
#define PIN_ENA  17 // ESP32 pin GPIO17 connected to the EN1 pin L298N

// Define UUIDs
#define SERVICE_UUID        "FFE0"
#define CHARACTERISTIC_UUID "FFE1"
volatile bool button0Down = false;   // true while pressed (based on D/U)
volatile bool button0Event = false; // true for a one-shot event to handle in loop()
// Global Pointers (Must be accessible by setup() and loop())
BLEServer* pServer = nullptr;
BLEService* pService = nullptr; // <--- NEW: Must track the service pointer
BLECharacteristic* pCharacteristic = nullptr; 
String msg = "B0:U";
int angle = 0;
int channel = 0;
double mag = 0.000;
char ctrlType = 'J';
class MyCallbacks : public BLECharacteristicCallbacks {
void onWrite(BLECharacteristic *pChar) override {
  size_t len = pChar->getLength();
  if (len == 0) return;

  const uint8_t* data = pChar->getData();
  String newMsg;
  for (size_t i = 0; i < len; i++) newMsg += (char)data[i];
  // 1) Split "J1:2,3.333" into left ("J1") and right ("2,3.333")
int colonPos = newMsg.indexOf(':');
if (colonPos < 0) return; // invalid format

String left = newMsg.substring(0, colonPos);     // "J1"
String right = newMsg.substring(colonPos + 1);  // "2,3.333"

// 2) From left, extract the integer after the letter 'J'
char letter = left[0]; // 'J' (if you expect it)
int int1 = left.substring(1).toInt(); // parses "1" -> 1

// 3) Split right into "2" and "3.333"
int commaPos = right.indexOf(',');
if (commaPos < 0) return; // invalid format

String part2Str = right.substring(0, commaPos);         // "2"
String part3Str = right.substring(commaPos + 1);        // "3.333"

int int2 = part2Str.toInt();     // 2
double dbl3 = part3Str.toFloat(); // 3.333 (float->double)
angle = int2;
mag = dbl3;
ctrlType = letter;
channel = int1;
msg = newMsg;          // update latest command

}
};
void setup() {
  pinMode(PIN_IN1, OUTPUT);
pinMode(PIN_IN2, OUTPUT);
pinMode(PIN_ENA, OUTPUT);
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, 0);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Initialize the device
  BLEDevice::init("XIAO_ESP32C6");

  // 1. Create Server (pServer is handled by init/create())
  pServer = BLEDevice::createServer();

  // 2. Create Service (Assigning pointer to global pService)
  // The return value of createService must be assigned as a pointer
  pService = pServer->createService(SERVICE_UUID);

  // 3. Create Characteristic (Assigning pointer to global pCharacteristic)
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  // Set initial value using the global pointer
  pCharacteristic->setValue("Hello World");
pCharacteristic->setCallbacks(new MyCallbacks());
  // 4. Start Service (Use the service pointer)
  pService->start();

  // Advertising setup remains the same...
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();

  Serial.println("Advertising started. Characteristic should be discoverable.");
}


void loop() {
  /*Serial.println(msg);
  Serial.println(angle);
  Serial.println(mag);
  Serial.println(channel);
  Serial.println(ctrlType);*/
  if (msg.equals(bpress)) {
      digitalWrite(LED_BUILTIN, LOW);
    } else{
      digitalWrite(LED_BUILTIN, HIGH);
    }
  int aMag = (int)(mag*200);
  if((ctrlType == 'J')&&(channel == 0)){
    if((angle >= 0)&&(angle <=180)){
      digitalWrite(PIN_IN1, HIGH); // control the motor's direction in clockwise
    digitalWrite(PIN_IN2, LOW);  // control the motor's direction in clockwise
    } else {
      digitalWrite(PIN_IN1, LOW); // control the motor's direction in clockwise
    digitalWrite(PIN_IN2, HIGH);  // control the motor's direction in clockwise
    }
    analogWrite(LED_BUILTIN, (200-aMag));
    
    analogWrite(PIN_ENA, (aMag));
    Serial.println(aMag);
    
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, 0);
  }

}
