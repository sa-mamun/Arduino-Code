#include "SPI.h"
#include "MFRC522.h"
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3231.h>

#define SS_PIN 10
#define RST_PIN 9

int temp_sensor = 5;       // Pin DS18B20 Sensor is connected to

float temperature = 0;      //Variable to store the temperature in
int lowerLimit = 34;      //define the lower threshold for the temperature
int upperLimit = 38;      //define the upper threshold for the temperature
float maxTemp = 0;
//String Date = "";
String Time = "";

DS3231  rtc(SDA, SCL);

OneWire oneWirePin(temp_sensor);

DallasTemperature sensors(&oneWirePin);

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

SoftwareSerial ArduinoUno(3,2);

void setup() {
    Serial.begin(115200);
    sensors.begin();
    ArduinoUno.begin(4800);
    SPI.begin();
    rfid.PCD_Init();
    rtc.begin();
    // The following lines can be uncommented to set the date and time
    //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
    //rtc.setTime(21, 53, 00);     // Set the time to 12:00:00 (24hr format)
    //rtc.setDate(12, 12, 2018);   // Set the date to January 1st, 2014
}

void loop() {

//-------------------For Temperature Sensor-----------------//

  sensors.requestTemperatures(); 
  
  temperature = sensors.getTempCByIndex(0);

  
  if(maxTemp < temperature){
    maxTemp = temperature;
    Serial.print("Temperature is ");
    Serial.println(maxTemp);
    delay(50);
  }

  //------------------For RFID Sensor----------------------//
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
//-------------Date------------//
  
  //Date = rtc.getDateStr();
  Time = rtc.getTimeStr();
  
  strID += String(maxTemp) + Time;
  maxTemp = 0.0;
  Serial.print("Tap card key: ");
  Serial.println(strID);
  //Serial.print("Date: ");
  //Serial.println(Date);
  ArduinoUno.print(strID);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  
}
