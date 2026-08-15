#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
String bpress = "B0:D\n";
// set motors
#define WeightPos  19
#define WeightNeg  18
#define WeightPWM  17 
#define ShellPos  2 
#define ShellNeg  1 
#define ShellPWM  0  
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
  pinMode(WeightPos, OUTPUT);
  pinMode(WeightNeg, OUTPUT);
  pinMode(WeightPWM, OUTPUT);
  pinMode(ShellPos, OUTPUT);
  pinMode(ShellNeg, OUTPUT);
  pinMode(ShellPWM, OUTPUT);
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
  digitalWrite(LED_BUILTIN, LOW);
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
  if((ctrlType == 'J')&&(channel == 0)){ // weight control
  int aMag = (int)(mag*200);
  if ((angle>0)&&(angle<90)){
        Serial.println("Top Right");
    analogWrite(WeightPWM, aMag);
    digitalWrite(WeightPos, HIGH); 
    digitalWrite(WeightNeg, LOW); 
  } else if ((angle>90)&&(angle<180)){
        Serial.println("top left");
    analogWrite(WeightPWM, aMag);
    digitalWrite(WeightPos, HIGH); 
    digitalWrite(WeightNeg, LOW); 
  } else if ((angle>180)&&(angle<270)){
        Serial.println("lower left");
    analogWrite(WeightPWM, aMag);
    digitalWrite(WeightPos, LOW); 
    digitalWrite(WeightNeg, HIGH); 
  } else if ((angle>270)&&(angle<360)) {
        Serial.println("lower right");
    analogWrite(WeightPWM, aMag);
    digitalWrite(WeightPos, LOW); 
    digitalWrite(WeightNeg, HIGH); 
  } else if (angle == 0){
    digitalWrite(WeightPos, HIGH); 
    digitalWrite(WeightNeg, LOW); 
    analogWrite(WeightPWM, aMag);
  } else {
    analogWrite(WeightPWM, 0);
  }
  
  } else if ((ctrlType == 'J')&&(channel == 1)) { // shell controll
      if ((angle>0)&&(angle<90)){
        Serial.println("Top Right");
    analogWrite(ShellPWM, aMag);
    digitalWrite(ShellPos, HIGH); 
    digitalWrite(ShellNeg, LOW); 
    Serial.print(angle);
  } else if ((angle>90)&&(angle<180)){
        Serial.println("top left");
    analogWrite(ShellPWM, aMag); 
    digitalWrite(ShellPos, HIGH); 
    digitalWrite(ShellNeg, LOW); 
  } else if ((angle>180)&&(angle<270)){
        Serial.println("lower left");
    analogWrite(ShellPWM, aMag); 
    digitalWrite(ShellPos, LOW); 
    digitalWrite(ShellNeg, HIGH); 
  } else if ((angle>270)&&(angle<360)) {
        Serial.println("lower right");
    analogWrite(ShellPWM, aMag);
    digitalWrite(ShellPos, LOW); 
    digitalWrite(ShellNeg, HIGH); 
  } else if (angle == 0){
    digitalWrite(ShellPos, HIGH); 
    digitalWrite(ShellNeg, LOW); 
    analogWrite(ShellPWM, aMag);
    
  } else {
    analogWrite(ShellPWM, 0);
  }
  }

}
