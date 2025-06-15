// Note: You can set whether your relay is active HIGH or LOW. Just search for DigitalWrite(relay) function.

// Blynk app connection
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "//use your own template ID"
#define BLYNK_TEMPLATE_NAME "//use your own template name"
#define BLYNK_AUTH_TOKEN "//use your own auth token from Blynk"

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

// ========== pin config for i/o ==========
#define RST_PIN 22
#define SS_PIN 5
#define buzzer 26
#define relay 27
#define reed 13

// define EEPROM size
#define EEPROM_SIZE 128
#define UID_ADDR 0

// RFID object define
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::Uid registeredUid;

// ========== global var ==========
char ssid[] = "//"; // SSID of WiFi to connect
char password[] = "//"; // Password of the WiFi
//BLYNK Token
char auth[] = "//"; // Your Blynk auth token (Same as above Blynk App Connection)

int globalStatus = 0;
bool cardScanned = false;
bool isWaitingForDoor = false;
unsigned long waitStartTime = 0;
const unsigned long maxWaitTime = 50000; // max time for door timeout

WiFiClientSecure client;

BlynkTimer timer; //untuk timer blynk terus menerus (periodik)

// compare RFID-scanned card ID to EEPROM registered card ID
bool compareUid(MFRC522::Uid uid1, MFRC522::Uid uid2){
  if (uid1.size != uid2.size) return false;
  for (byte i = 0; i < uid1.size; i++) {
    if (uid1.uidByte[i] != uid2.uidByte[i]) return false;
  }
  return true;
}

// ========== saving card ID after scan ==========
void saveUidToEEPROM(MFRC522::Uid uid){
  EEPROM.write(0, uid.size);
  for (byte i = 0; i < uid.size; i++){
    EEPROM.write(i + 1, uid.uidByte[i]);
  }
  EEPROM.commit();
}

// ========== Load card ID from EEPROM ==========
void loadUidFromEEPROM(MFRC522::Uid &uid){
  uid.size = EEPROM.read(0);
  for (byte i = 0; i < uid.size; i++){
    uid.uidByte[i] = EEPROM.read(i + 1);
  }
}

// ========== check if EEPROM is occupied ==========
bool isUidStored(){
  byte size = EEPROM.read(0);
  return (size >= 4 && size <= 10);
}

// main function to registering RFID card. if there is no ID in EEPROM, then register the card
void cekKartuRegister(){

   Serial.println();
  Serial.println("Scan kartu atau registrasi...");
  if (!isUidStored()){
    while (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) delay(1000);
    registeredUid = rfid.uid;
    saveUidToEEPROM(rfid.uid);
    tone(buzzer, 200, 50);
    delay(200);
    tone(buzzer, 200, 50);
    delay(200);
    tone(buzzer, 200, 50);
    delay(200);
    delay(1000);
    Serial.println("Kartu berhasil diregistrasi.");
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  } else{
    loadUidFromEEPROM(registeredUid);
    Serial.println("UID berhasil dimuat dari EEPROM.");
  }
  return;
}

// waiting for door closed function, and timeout for the door if not closed
void waitForDoorToClose(){

  if (isWaitingForDoor){
    if (digitalRead(reed) != LOW){
      Serial.println("Pintu tertutup.");
      delay(1500);
      digitalWrite(relay, HIGH);
      globalStatus = 0;
      isWaitingForDoor = false;
    } else if (millis() - waitStartTime > maxWaitTime){
      Serial.println("Waktu tunggu habis. Menutup pintu otomatis.");
      delay(1500);
      digitalWrite(relay, HIGH);
      globalStatus = 0;
      isWaitingForDoor = false;
    }
  }
}

// check if any RFID card has been scanned
void checkRFID(){
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    cardScanned = true;
  }
}

// open door using Blynk app
BLYNK_WRITE(V2) {
  int value = param.asInt();
  if (value == 1 && globalStatus == 0){
    Serial.println("Buka pintu via push button Blynk...");
    digitalWrite(relay, LOW); 
    tone(buzzer, 650, 100);   
    globalStatus = 1;
    isWaitingForDoor = true;
    delay(5000);
    Serial.println("Menunggu pintu tertutup...");
    waitStartTime = millis(); // start timeout timer
    waitForDoorToClose(); // timeout for door
  }
}

// reset ID function via Blynk app
BLYNK_WRITE(V1) {
  int value = param.asInt();
  if (value == 1){
    Serial.println("Reset UID via Blynk ditekan!");

    // EEPROM Reset
    for (int i = 0; i < EEPROM_SIZE; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();

    tone(buzzer, 500, 100);
    tone(buzzer, 680, 100);
    delay(1000);
    Serial.println("UID berhasil dihapus. Silakan scan kartu baru...");

    cekKartuRegister();
  }
}

// main function
void fungsiUtama(){

  if (!cardScanned) return;

  if (compareUid(rfid.uid, registeredUid)){
    tone(buzzer, 600, 100);
    digitalWrite(relay, LOW);
    globalStatus = 1;
    Serial.println("Pintu dibuka!");
    Blynk.logEvent("pintu_terbuka");

    delay(5000);
    Serial.println("Menunggu pintu tertutup...");
    isWaitingForDoor = true;
    waitStartTime = millis(); // start timer
  } else {
    tone(buzzer, 500, 100);
    delay(200);
    tone(buzzer, 500, 100);
    delay(200);
    Serial.println("Akses ditolak, UID tidak cocok.");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  cardScanned = false;
}

// ========== Setup ==========
void setup(){

  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  EEPROM.begin(EEPROM_SIZE);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.println();
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password, "blynk.cloud", 80);

  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(reed, INPUT);

  cekKartuRegister();
}

// ========== main loop ==========
void loop(){

  Blynk.run();
  timer.run();
  checkRFID();
  fungsiUtama();
  waitForDoorToClose();

}
